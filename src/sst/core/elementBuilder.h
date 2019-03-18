// Copyright 2009-2018 NTESS. Under the terms
// of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
// 
// Copyright (c) 2009-2018, NTESS
// All rights reserved.
// 
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#ifndef SST_CORE_ELEMENTBUILDER_H
#define SST_CORE_ELEMENTBUILDER_H

#include <sst/core/sst_types.h>
#include <sst/core/warnmacros.h>
#include <sst/core/params.h>

#include <string>
#include <vector>


namespace SST {

/****************************************************
   Base classes for templated creation of classes
   derived from a base API
*****************************************************/

template <class T, typename... ARGS>
class  DerivedCreatorBase {
public:
    virtual T* create(ARGS... args) = 0;
    virtual ~DerivedCreatorBase() {}
};

template <class T, typename... ARGS>
class ElementBuilder {

    static const std::string name;
    
public:

    typedef DerivedCreatorBase<T, ARGS...> creator_t;

    typedef std::map<std::string, creator_t*> creator_map_t;
    static creator_map_t* creator_map;

    static const std::string& getName() {
        return name;
    }

    // Add a derived class to the Builder object
    static bool register_derived_element(const std::string& name, creator_t* creator) {
        if ( !creator_map ) {
            creator_map = new creator_map_t;
        }
        (*creator_map)[name] = creator;
        return true;
    }

    // Build derived class based on an already registered name
    static T* build(const std::string& name, ARGS... args) {
        auto it = (*creator_map).find(name);
        if ( it == creator_map->end() ) {
            Output output("@R, @I (@t): " /*prefix*/, 0, 0, Output::STDOUT);
            output.fatal(CALL_INFO, 1, "ERROR: SubComponent %s not registered as a %s API\n",name.c_str(),T::ELI_getElementBuilderName().c_str());
            exit(1);
        }
        creator_t* creator = it->second;
        
        T* ret = creator->create(args...);
        return ret;
    }
};

template <class T, typename... ARGS> std::map<std::string, DerivedCreatorBase<T,ARGS...>*>* ElementBuilder<T,ARGS...>::creator_map = nullptr;



template <class T, typename P, typename... ARGS>
class DerivedCreator : public DerivedCreatorBase<P, ARGS...> {

public:
    static const bool registered;
    DerivedCreator() {}

    
    P* create(ARGS... args) {
        // using expander = int[];
        // std::cout << std::hex;
        // (void)expander{0, (void(std::cout << ',' << std::forward<ARGS>(args)), 0)...};
        // std::cout << std::dec;
        // std::cout << std::endl;
        
        return new T(args...);
    }
};



// Macros to add a builder to a base class and to register a derived
// class with the builder


// This macro is used by the base class to declare that they want a
// builder.  All that is passed in is the fully qualified type name of
// the class calling the macro and the type signature for the
// constructor to be used by derived classes.
#define SST_ELI_DECLARE_BUILDER(type,...)           \
    static ElementBuilder<type,__VA_ARGS__> ELI_ElementBuilder;   \
    static std::string ELI_getElementBuilderName() { return #type; } \
    template<class T>                                               \
    using ELI_CreatorType = DerivedCreator<T,type,__VA_ARGS__>;

#define SST_ELI_REGISTER_DERIVED_BUILDER(name,child,parent) \
    static bool ELI_isBuilderRegistered() {                     \
        return parent::ELI_CreatorType<child>::registered;  \
    } \
    static std::string ELI_getBuilderName() {    \
        return name; \
    }

// template<class P, typename... ARGS> bool init(std::string name, DerivedCreatorBase<P, ARGS...>* creator) {
//     bool ret = P::ELI_ElementBuilder.register_derived_element(name,creator);
//     return ret;
// }        
// template<class T, typename P, typename... ARGS> const bool DerivedCreator<T,P,ARGS...>::registered = init<P,ARGS...>(T::ELI_getBuilderName(), new DerivedCreator<T,P,ARGS...>());

template<class T, typename P, typename... ARGS> const bool DerivedCreator<T,P,ARGS...>::registered = P::ELI_ElementBuilder.register_derived_element(T::ELI_getBuilderName(),new DerivedCreator<T,P,ARGS...>());

template <class T, typename... ARGS> const std::string ElementBuilder<T,ARGS...>::name = T::ELI_getElementBuilderName();

} //namespace SST

#endif // SST_CORE_ELEMENTBUILDER_H
