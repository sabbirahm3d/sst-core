// Copyright 2009-2016 Sandia Corporation. Under the terms
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
// 
// Copyright (c) 2009-2016, Sandia Corporation
// All rights reserved.
// 
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.


#include "sst_config.h"
// #include "sst/core/serialization.h"

#include <sst/core/componentInfo.h>
#include <sst/core/linkMap.h>

namespace SST {

ComponentInfo::ComponentInfo(ComponentId_t id, const std::string &name, const std::string &type, const Params *params, LinkMap* link_map) :
    id(id),
    name(name),
    type(type),
    link_map(link_map),
    params(params),
    component(NULL),
    enabledStats(NULL),
    statParams(NULL)
{ }


ComponentInfo::ComponentInfo(const ConfigComponent *ccomp, LinkMap* link_map) :
    id(ccomp->id),
    name(ccomp->name),
    type(ccomp->type),
    link_map(link_map),
    component(NULL),
    params(&ccomp->params),
    enabledStats(NULL),
    statParams(NULL)
{
    for ( auto &sc : ccomp->subComponents ) {
        subComponents.emplace(sc.first, ComponentInfo(&sc.second, new LinkMap()));
    }
}

ComponentInfo::ComponentInfo(ComponentInfo &&o) :
    id(o.id),
    name(std::move(o.name)),
    type(std::move(o.type)),
    link_map(o.link_map),
    component(o.component),
    params(o.params),
    enabledStats(std::move(o.enabledStats)),
    statParams(std::move(o.statParams))
{
    o.link_map = NULL;
    o.component = NULL;
}


ComponentInfo::~ComponentInfo() {
    if ( link_map ) delete link_map;
    if ( component ) delete component;
}

} // namespace SST

// BOOST_CLASS_EXPORT_IMPLEMENT(SST::InitQueue)
