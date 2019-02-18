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

#include <sst_config.h>
#include <sst/core/warnmacros.h>

#include <string>

#include <sst/core/baseComponent.h>
#include <sst/core/component.h>
#include <sst/core/subcomponent.h>
#include <sst/core/unitAlgebra.h>
#include <sst/core/factory.h>
#include <sst/core/link.h>
#include <sst/core/linkMap.h>
#include <sst/core/simulation.h>
#include <sst/core/timeConverter.h>
#include <sst/core/timeLord.h>
#include <sst/core/unitAlgebra.h>
#include <sst/core/sharedRegion.h>

namespace SST {


// BaseComponent::BaseComponent() :
//     defaultTimeBase(NULL), my_info(NULL),
//     sim(Simulation::getSimulation()),
//     currentlyLoadingSubComponent(NULL),
//     parent(NULL)
// {}

// BaseComponent::BaseComponent(Component* parent) :
//     sim(Simulation::getSimulation()),
//     defaultTimeBase(NULL),
//     my_info(NULL),
//     currentlyLoadingSubComponent(NULL)
// {
// }

BaseComponent::BaseComponent(ComponentId_t id) :
    sim(Simulation::getSimulation()),
    my_info(Simulation::getSimulation()->getComponentInfo(id)),
    currentlyLoadingSubComponent(NULL),
    defaultTimeBase(NULL)
{
    my_info->component = this;
}



BaseComponent::~BaseComponent()
{
}

void
BaseComponent::setDefaultTimeBaseForLinks(TimeConverter* tc) {
    LinkMap* myLinks = my_info->getLinkMap();
    if (NULL != myLinks) {
        for ( std::pair<std::string,Link*> p : myLinks->getLinkMap() ) {
            if ( NULL == p.second->getDefaultTimeBase() ) {
                p.second->setDefaultTimeBase(tc);
            }
        }
    }

    // Need to look through my child subcomponents and for all
    // anonymously loaded subcomponents, set the default time base for
    // any links they have.  These links would have been moved from
    // the parent to the child.
    for ( auto &sub : my_info->subComponents ) {
        if ( sub.second.isAnonymousSubComponent() ) {
            sub.second.component->setDefaultTimeBaseForLinks(tc);
        }
    }
    
}


TimeConverter* BaseComponent::registerClock( std::string freq, Clock::HandlerBase* handler, bool regAll) {
    TimeConverter* tc = getSimulation()->registerClock(freq, handler, CLOCKPRIORITY);

    // if regAll is true set tc as the default for the component and
    // for all the links
    if ( regAll ) {
        setDefaultTimeBaseForLinks(tc);
        defaultTimeBase = tc;
    }
    return tc;
}

TimeConverter* BaseComponent::registerClock( const UnitAlgebra& freq, Clock::HandlerBase* handler, bool regAll) {
    TimeConverter* tc = getSimulation()->registerClock(freq, handler, CLOCKPRIORITY);

    // if regAll is true set tc as the default for the component and
    // for all the links
    if ( regAll ) {
        setDefaultTimeBaseForLinks(tc);
        defaultTimeBase = tc;
    }
    return tc;
}

Cycle_t BaseComponent::reregisterClock( TimeConverter* freq, Clock::HandlerBase* handler) {
    return getSimulation()->reregisterClock(freq, handler, CLOCKPRIORITY);
}

Cycle_t BaseComponent::getNextClockCycle( TimeConverter* freq ) {
    return getSimulation()->getNextClockCycle(freq, CLOCKPRIORITY);
}

void BaseComponent::unregisterClock(TimeConverter *tc, Clock::HandlerBase* handler) {
    getSimulation()->unregisterClock(tc, handler, CLOCKPRIORITY);
}

TimeConverter* BaseComponent::registerOneShot( std::string timeDelay, OneShot::HandlerBase* handler) {
    return getSimulation()->registerOneShot(timeDelay, handler, ONESHOTPRIORITY);
}

TimeConverter* BaseComponent::registerOneShot( const UnitAlgebra& timeDelay, OneShot::HandlerBase* handler) {
    return getSimulation()->registerOneShot(timeDelay, handler, ONESHOTPRIORITY);
}

TimeConverter* BaseComponent::registerTimeBase( std::string base, bool regAll) {
    TimeConverter* tc = getSimulation()->getTimeLord()->getTimeConverter(base);

    // if regAll is true set tc as the default for the component and
    // for all the links
    if ( regAll ) {
        setDefaultTimeBaseForLinks(tc);
        defaultTimeBase = tc;
    }
    return tc;
}

TimeConverter*
BaseComponent::getTimeConverter( const std::string& base )
{
    return getSimulation()->getTimeLord()->getTimeConverter(base);
}

TimeConverter*
BaseComponent::getTimeConverter( const UnitAlgebra& base )
{
    return getSimulation()->getTimeLord()->getTimeConverter(base);
}


bool
BaseComponent::isPortConnected(const std::string &name) const
{
    return (my_info->getLinkMap()->getLink(name) != NULL);
}


// Looks at parents' shared ports and returns the link connected to
// the port of the correct name in one of my parents. If I find the
// correct link, and it hasn't been configured yet, I return it to the
// child and remove it from my linkmap.  The child will insert it into
// their link map.
Link*
BaseComponent::getLinkFromParentSharedPort(const std::string& port)
{
    LinkMap* myLinks = my_info->getLinkMap();

    // See if the link is found, and if not see if my parent shared
    // their ports with me
    
    if ( NULL != myLinks ) {
        Link* tmp = myLinks->getLink(port);
        if ( NULL != tmp ) {
            // Found the link in my linkmap

            // Check to see if it has been configured.  If not, remove
            // it from my link map and return it to the child.
            if ( !tmp->isConfigured() ) {
                myLinks->removeLink(port);
                return tmp;
            }
        }
    }

    // If we get here, we didn't find the link.  Check to see if my
    // parent shared with me and if so, call
    // getLinkFromParentSharedPort on them
        
    if ( my_info->sharesPorts() ) {
        return my_info->parent_info->component->getLinkFromParentSharedPort(port);
    }
    else {
        return NULL;
    }    
}


Link*
BaseComponent::configureLink(std::string name, TimeConverter* time_base, Event::HandlerBase* handler)
{
    LinkMap* myLinks = my_info->getLinkMap();


    Link* tmp = NULL;
    
    // If I have a linkmap, check to see if a link was connected to
    // port "name"
    if ( NULL != myLinks ) {
        tmp = myLinks->getLink(name);
    }

    // If tmp is NULL, then I didn't have the port connected, check
    // with parents if sharing is turned on
    if ( NULL == tmp ) {
        if ( my_info->sharesPorts() ) {
            tmp = my_info->parent_info->component->getLinkFromParentSharedPort(name);

            // If I got a link from my parent, I need to put it in my
            // link map
            if ( NULL != tmp ) {
                if ( NULL == myLinks ) {
                    myLinks = new LinkMap();
                    my_info->link_map = myLinks;
                }
                myLinks->insertLink(name,tmp);
                // Need to set the link's defaultTimeBase to NULL,
                // except in the case of this being an Anonymously
                // loadeed SubComponent, then for backward
                // compatibility, we leave it as is.
                if ( !my_info->isAnonymousSubComponent() ) {
                    tmp->setDefaultTimeBase(NULL);
                }
            }
        }
    }

    // If I got a link, configure it
    if ( NULL != tmp ) {
        
        // If no functor, this is a polling link
        if ( handler == NULL ) {
            tmp->setPolling();
        }
        tmp->setFunctor(handler);
        if ( NULL != time_base ) tmp->setDefaultTimeBase(time_base);
        else tmp->setDefaultTimeBase(defaultTimeBase);
        tmp->setAsConfigured();
#ifdef __SST_DEBUG_EVENT_TRACKING__
        tmp->setSendingComponentInfo(my_info->getName(), my_info->getType(), name);
#endif        
    }

    return tmp;
}

Link*
BaseComponent::configureLink(std::string name, std::string time_base, Event::HandlerBase* handler)
{
    return configureLink(name,getSimulation()->getTimeLord()->getTimeConverter(time_base),handler);
}

Link*
BaseComponent::configureLink(std::string name, Event::HandlerBase* handler)
{
    return configureLink(name,NULL,handler);
}

void
BaseComponent::addSelfLink(std::string name)
{
    LinkMap* myLinks = my_info->getLinkMap();
    myLinks->addSelfPort(name);
    if ( myLinks->getLink(name) != NULL ) {
        printf("Attempting to add self link with duplicate name: %s\n",name.c_str());
        abort();
    }

    Link* link = new SelfLink();
    // Set default time base to the component time base
    link->setDefaultTimeBase(defaultTimeBase);
    myLinks->insertLink(name,link);

}

Link*
BaseComponent::configureSelfLink( std::string name, TimeConverter* time_base, Event::HandlerBase* handler)
{
    addSelfLink(name);
    return configureLink(name,time_base,handler);
}

Link*
BaseComponent::configureSelfLink( std::string name, std::string time_base, Event::HandlerBase* handler)
{
    addSelfLink(name);
    return configureLink(name,time_base,handler);
}

Link*
BaseComponent::configureSelfLink( std::string name, Event::HandlerBase* handler)
{
    addSelfLink(name);
    return configureLink(name,handler);
}

SimTime_t BaseComponent::getCurrentSimTime(TimeConverter *tc) const {
    return tc->convertFromCoreTime(getSimulation()->getCurrentSimCycle());
}

SimTime_t BaseComponent::getCurrentSimTime(std::string base) {
    return getCurrentSimTime(getSimulation()->getTimeLord()->getTimeConverter(base));

}

SimTime_t BaseComponent::getCurrentSimTimeNano() const {
    return getCurrentSimTime(getSimulation()->getTimeLord()->getNano());
}

SimTime_t BaseComponent::getCurrentSimTimeMicro() const {
    return getCurrentSimTime(getSimulation()->getTimeLord()->getMicro());
}

SimTime_t BaseComponent::getCurrentSimTimeMilli() const {
    return getCurrentSimTime(getSimulation()->getTimeLord()->getMilli());
}



Module*
BaseComponent::loadModule(std::string type, Params& params)
{
    return Factory::getFactory()->CreateModule(type,params);
}

Module*
BaseComponent::loadModuleWithComponent(std::string type, Component* comp, Params& params)
{
    return Factory::getFactory()->CreateModuleWithComponent(type,comp,params);
}


/* Old ELI style */
SubComponent*
BaseComponent::loadSubComponent(std::string type, Component* comp, Params& params)
{
    // /* Old Style SubComponents end up with their parent's Id, name, etc. */
    // ComponentInfo *sub_info = new ComponentInfo(type, &params, my_info);

    // /* By "magic", the new component will steal ownership of this pointer */
    // currentlyLoadingSubComponent = sub_info;
    ComponentId_t cid = comp->currentlyLoadingSubComponentID;
    comp->currentlyLoadingSubComponentID = my_info->addComponentDefinedSubComponent(my_info, type, "ANONYMOUS", 0,
          ComponentInfo::SHARE_PORTS | ComponentInfo::SHARE_STATS | ComponentInfo::INSERT_STATS | ComponentInfo::IS_ANONYMOUS_SUBCOMPONENT);
    
    SubComponent* ret = Factory::getFactory()->CreateSubComponent(type,comp,params);
    comp->currentlyLoadingSubComponentID = cid;

    // sub_info->setComponent(ret);
    // currentlyLoadingSubComponent = NULL;
    return ret;
}

// SubComponent*
// BaseComponent::loadAnonymousSubComponent(std::string type, Params& params)
// {

//     ComponentInfo *sub_info = new ComponentInfo(type, &params, my_info);

//     /* By "magic", the new component will steal ownership of this pointer */
//     currentlyLoadingSubComponent = sub_info;

//     SubComponent* ret = Factory::getFactory()->CreateSubComponent(type,getTrueComponent(),params);
//     sub_info->setComponent(ret);
//     currentlyLoadingSubComponent = NULL;

//     return ret;    
// }

Component*
BaseComponent::getTrueComponent() const {
    // Walk up the parent tree until we hit the base Component.  We
    // know we're the base Component when parent is NULL.
    ComponentInfo* info = my_info;
    while ( info->parent_info != NULL ) info = info->parent_info;
    return static_cast<Component* const>(info->component);
}

/* New ELI style */
SubComponent*
BaseComponent::loadNamedSubComponent(std::string name) {
    Params empty;
    return loadNamedSubComponent(name, empty);
}

SubComponent*
BaseComponent::loadNamedSubComponent(std::string name, Params& params) {
    // Get list of ComponentInfo objects and make sure that there is
    // only one SubComponent put into this slot
    const std::map<ComponentId_t,ComponentInfo>& subcomps = my_info->getSubComponents();
    int sub_count = 0;
    for ( auto &ci : subcomps ) {
        if ( ci.second.getSlotName() == name ) {
            sub_count++;
        }
    }
    
    if ( sub_count > 1 ) {
        SST::Output outXX("SubComponentSlotWarning: ", 0, 0, Output::STDERR);
        outXX.fatal(CALL_INFO, 1, "Error: ComponentSlot \"%s\" in component \"%s\" only allows for one SubComponent, %d provided.\n",
                    name.c_str(), my_info->getType().c_str(), sub_count);
    }
    
    return loadNamedSubComponent(name, 0, params);
}

SubComponent*
BaseComponent::loadNamedSubComponent(std::string name, int slot_num) {
    Params empty;
    return loadNamedSubComponent(name, slot_num, empty);
}

// Private
SubComponent*
BaseComponent::loadNamedSubComponent(std::string name, int slot_num, Params& params)
{
    if ( !Factory::getFactory()->DoesSubComponentSlotExist(my_info->type, name) ) {
        SST::Output outXX("SubComponentSlotWarning: ", 0, 0, Output::STDERR);
        outXX.output(CALL_INFO, "Warning: SubComponentSlot \"%s\" is undocumented.\n", name.c_str());
    }
    
    ComponentInfo* sub_info = my_info->findSubComponent(name,slot_num);
    if ( sub_info == NULL ) return NULL;
    sub_info->share_flags = ComponentInfo::SHARE_NONE;
    sub_info->parent_info = my_info;
    
    // ComponentInfo *oldLoadingSubcomponent = getTrueComponent()->currentlyLoadingSubComponent;
    // // ComponentInfo *sub_info = &(infoItr->second);
    // getTrueComponent()->currentlyLoadingSubComponent = sub_info;

    ComponentId_t cid = getTrueComponent()->currentlyLoadingSubComponentID;
    getTrueComponent()->currentlyLoadingSubComponentID = sub_info->id;
        
    Params myParams;
    if ( sub_info->getParams() != NULL )
        myParams.insert(*sub_info->getParams());
    myParams.insert(params);

    SubComponent* ret = Factory::getFactory()->CreateSubComponent(sub_info->getType(), getTrueComponent(), myParams);
    sub_info->setComponent(ret);

    getTrueComponent()->currentlyLoadingSubComponentID = cid;
    return ret;
}

SubComponentSlotInfo*
BaseComponent::getSubComponentSlotInfo(std::string name, bool fatalOnEmptyIndex) {
    SubComponentSlotInfo* info = new SubComponentSlotInfo(this, name);
    if ( info->getMaxPopulatedSlotNumber() < 0 ) {
        // Nothing registered on this slot
        delete info;
        return NULL;
    }
    if ( !info->isAllPopulated() && fatalOnEmptyIndex ) {
        Simulation::getSimulationOutput().
            fatal(CALL_INFO,1,
                  "SubComponent slot %s requires a dense allocation of SubComponents and did not get one.\n",
                  name.c_str());
    }
    return info;
}

// SubComponentBuilderBase*
// BaseComponent::getSubComponentBuilder(std::string type)
// {
//     return Factory::getFactory()->getSubComponentBuilder(type);
// }

bool
BaseComponent::doesSubComponentExist(std::string type)
{
    return Factory::getFactory()->doesSubComponentExist(type);
}

SharedRegion* BaseComponent::getLocalSharedRegion(const std::string &key, size_t size)
{
    SharedRegionManager *mgr = Simulation::getSharedRegionManager();
    return mgr->getLocalSharedRegion(key, size);
}


SharedRegion* BaseComponent::getGlobalSharedRegion(const std::string &key, size_t size, SharedRegionMerger *merger)
{
    SharedRegionManager *mgr = Simulation::getSharedRegionManager();
    return mgr->getGlobalSharedRegion(key, size, merger);
}



uint8_t BaseComponent::getComponentInfoStatisticEnableLevel(const std::string &statisticName) const
{
    const std::string& type = getStatisticOwner()->my_info->getType();
    return Factory::getFactory()->GetComponentInfoStatisticEnableLevel(type, statisticName);
}

std::string BaseComponent::getComponentInfoStatisticUnits(const std::string &statisticName) const
{
    const std::string& type = getStatisticOwner()->my_info->getType();
    return Factory::getFactory()->GetComponentInfoStatisticUnits(type, statisticName);
}


} // namespace SST


