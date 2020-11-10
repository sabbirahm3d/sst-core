cmake_minimum_required(VERSION 3.12)
project(SST_CORE CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
# CC = ${MPICC}
# CXX = ${MPICXX}

# AM_CPPFLAGS =  \
#     -DTIXML_USE_STL \
#     -DSST_BUILDING_CORE=1 \
#     ${ZOLTAN_CPPFLAGS} \
#     ${PYTHON_CPPFLAGS} \
#     ${LTDLINCL}

# DIST_SUBDIRS = libltdl .
# SUBDIRS = libltdl .
# EXTRA_DIST = mainpage.dox eli/README.md

# sstdir = ${includedir}/sst/core
set(NOBASE_DIST_SST_HEADERS
    activityQueue.h
    action.h
    activity.h
    clock.h
    baseComponent.h
    component.h
    componentExtension.h
    componentInfo.h
    config.h
    configGraph.h
    configGraphOutput.h
    cfgoutput/pythonConfigOutput.h
    cfgoutput/dotConfigOutput.h
    cfgoutput/xmlConfigOutput.h
    cfgoutput/jsonConfigOutput.h
    decimal_fixedpoint.h
    env/envquery.h
    env/envconfig.h
    elemLoader.h
    event.h
    exit.h
    factory.h
    from_string.h
    heartbeat.h
    initQueue.h
    link.h
    linkMap.h
    linkPair.h
    mempool.h
    memuse.h
    iouse.h
    module.h
    objectComms.h
    oneshot.h
    output.h
    params.h
    pollingLinkQueue.h
    profile.h
    rankInfo.h
    rankSyncParallelSkip.h
    rankSyncSerialSkip.h
    simulation.h
    sparseVectorMap.h
    sst_types.h
    sstinfo.h
    sstpart.h
    stopAction.h
    stringize.h
    subcomponent.h
    syncBase.h
    syncManager.h
    syncQueue.h
    threadSync.h
    threadSyncSimpleSkip.h
    threadSyncQueue.h
    sharedRegion.h
    sharedRegionImpl.h
    timeConverter.h
    timeLord.h
    timeVortex.h
    math/sqrt.h
    uninitializedQueue.h
    unitAlgebra.h
    part/sstpart.h
    eli/elementinfo.h
    eli/elibase.h
    eli/categoryInfo.h
    eli/defaultInfo.h
    eli/elementbuilder.h
    eli/interfaceInfo.h
    eli/paramsInfo.h
    eli/portsInfo.h
    eli/statsInfo.h
    eli/subcompSlotInfo.h
    serialization/ser_ptr_type.h
    serialization/serializable.h
    serialization/serializable_fwd.h
    serialization/serialize.h
    serialization/serialize_array.h
    serialization/serialize_buffer_accessor.h
    serialization/serialize_deque.h
    serialization/serialize_list.h
    serialization/serialize_map.h
    serialization/serialize_packer.h
    serialization/serialize_serializable.h
    serialization/serialize_set.h
    serialization/serialize_sizer.h
    serialization/serialize_string.h
    serialization/serialize_unpacker.h
    serialization/serialize_vector.h
    serialization/serializer.h
    serialization/serializer_fwd.h
    serialization/statics.h
    interfaces/TestEvent.h
    interfaces/stringEvent.h
    interfaces/simpleMem.h
    interfaces/simpleNetwork.h
    interprocess/tunneldef.h
    interprocess/mmapparent.h
    interprocess/mmapchild_pin3.h
    interprocess/shmchild.h
    interprocess/shmparent.h
    interprocess/circularBuffer.h
    interprocess/sstmutex.h
    interprocess/ipctunnel.h
    rng/sstrng.h
    rng/marsaglia.h
    rng/poisson.h
    rng/mersenne.h
    rng/xorshift.h
    rng/distrib.h
    rng/discrete.h
    rng/gaussian.h
    rng/expon.h
    rng/constant.h
    rng/uniform.h
    statapi/statengine.h
    statapi/statgroup.h
    statapi/statoutput.h
    statapi/statfieldinfo.h
    statapi/statoutputconsole.h
    statapi/statuniquecount.h
    statapi/statoutputtxt.h
    statapi/statoutputcsv.h
    statapi/statoutputjson.h
    statapi/statoutputhdf5.h
    statapi/statbase.h
    statapi/stathistogram.h
    statapi/stataccumulator.h
    statapi/statnull.h
    threadsafe.h
    cputimer.h
    warnmacros.h
    model/element_python.h
)

set(NOBASE_NODIST_SST_HEADERS
    build_info.h
    sst_config.h
)

set(SST_CORE_SOURCES
    action.cc
    clock.cc
    baseComponent.cc
    component.cc
    componentExtension.cc
    componentInfo.cc
    config.cc
    configGraph.cc
    cfgoutput/pythonConfigOutput.cc
    cfgoutput/dotConfigOutput.cc
    cfgoutput/xmlConfigOutput.cc
    cfgoutput/jsonConfigOutput.cc
    env/envquery.cc
    env/envconfig.cc
    eli/elibase.cc
    eli/elementinfo.cc
    elemLoader.cc
    event.cc
    exit.cc
    factory.cc
    heartbeat.cc
    initQueue.cc
    link.cc
    memuse.cc
    oneshot.cc
    output.cc
    params.cc
    pollingLinkQueue.cc
    rankSyncParallelSkip.cc
    rankSyncSerialSkip.cc
    simulation.cc
    subcomponent.cc
    syncBase.cc
    syncManager.cc
    syncQueue.cc
    threadSync.cc
    threadSyncSimpleSkip.cc
    sharedRegion.cc
    timeLord.cc
    uninitializedQueue.cc
    unitAlgebra.cc
    module.cc
    sstpart.cc
    timeVortex.cc
    serialization/serializable.cc
    serialization/serialize_serializable.cc
    serialization/serializer.cc
    serialization/statics.cc
    interfaces/TestEvent.cc
    interfaces/simpleMem.cc
    interfaces/simpleNetwork.cc
    interprocess/ipctunnel.cc
    interprocess/mmapparent.cc
    interprocess/shmparent.cc
    rng/sstrng.cc
    rng/marsaglia.cc
    rng/mersenne.cc
    rng/xorshift.cc
    statapi/statengine.cc
    statapi/statgroup.cc
    statapi/statoutput.cc
    statapi/statfieldinfo.cc
    statapi/statoutputconsole.cc
    statapi/statoutputtxt.cc
    statapi/statoutputcsv.cc
    statapi/statoutputjson.cc
    statapi/statbase.cc
    cputimer.cc
    iouse.cc
)

set(BIN_PROGRAMS sst sst-info sst-config sst-register)
set(LIBEXEC_PROGRAMS sstsim.x sstinfo.x)

set(SST_INFO_SOURCES
    bootsstinfo.cc
    bootshared.cc
    bootshared.h
    env/envquery.h
    env/envconfig.h
    env/envquery.cc
    env/envconfig.cc
)
add_executable(sst-info ${SST_INFO_SOURCES})

set(SST_SOURCES
    bootsst.cc
    bootshared.cc
    bootshared.h
    env/envquery.h
    env/envconfig.h
    env/envquery.cc
    env/envconfig.cc
)
add_executable(sst ${SST_SOURCES})

set(SST_CONFIG_SOURCES
    sstconfigtool.cc
    env/envquery.h
    env/envconfig.h
    env/envquery.cc
    env/envconfig.cc
)
add_executable(sst-config ${SST_CONFIG_SOURCES})

set(SST_REGISTER_SOURCES
    sstregistertool.cc
    env/envquery.h
    env/envconfig.h
    env/envquery.cc
    env/envconfig.cc
)
add_executable(sst-register ${SST_REGISTER_SOURCES})

set(SSTSIM_X_SOURCES
    main.cc
    ${SST_CORE_SOURCES}
)

set(SSTINFO_X_SOURCES
    sstinfo.cc
    ${SST_CORE_SOURCES}
    ${sst_xml_sources}
)

set(SSTSIM_X_LDADD
    ${LIBLTDL}
    ${PYTHON_LIBS}
    ${ZOLTAN_LIB}
    ${PARMETIS_LIB}
    ${MPILIBS}
    ${TCMALLOC_LIB}
    -lm
)

set(SSTSIM_X_LDFLAGS
    ${PARMETIS_LDFLAGS}
    ${ZOLTAN_LDFLAGS}
    ${TCMALLOC_LDFLAGS}
    ${PYTHON_LDFLAGS}
    -export-dynamic
    ${SST_LTLIBS_ELEMLIBS}
)

set(SSTINFO_X_LDADD
    ${LIBLTDL}
    ${ZOLTAN_LIB}
    ${PARMETIS_LIB}
    ${MPILIBS}
    ${TCMALLOC_LIB}
    -lm
)

set(SSTINFO_X_LDFLAGS
    ${PARMETIS_LDFLAGS}
    ${ZOLTAN_LDFLAGS}
    ${TCMALLOC_LDFLAGS}
    ${PYTHON_LDFLAGS}
    -export-dynamic
    ${SST_LTLIBS_ELEMLIBS}
)

# include tinyxml/Makefile.inc
# include part/Makefile.inc
# include model/Makefile.inc
# include impl/Makefile.inc

if (NOT SST_ENABLE_PREVIEW_BUILD)
    set(NOBASE_DIST_SST_HEADERS ${NOBASE_DIST_SST_HEADERS} "${deprecated_NOBASE_DIST_SST_HEADERS}")
    set(SST_CORE_SOURCES ${SST_CORE_SOURCES} "${deprecated_SST_CORE_SOURCES}")
endif()

set(SST_CORE_SOURCES ${SST_CORE_SOURCES} "${SST_CORE_PYTHON_SOURCES}")
set(NOBASE_DIST_SST_HEADERS ${NOBASE_DIST_SST_HEADERS} "${SST_CORE_PYTHON_HEADERS}")

if (NOT SST_COMPILE_OSX)
    set(SSTSIM_X_LDADD ${SSTSIM_X_LDADD} "-lrt")
    set(SSTINFO_X_LDADD ${SSTINFO_X_LDADD} "-lrt")
endif()

if (USE_LIBZ)
    set(SSTSIM_X_LDADD ${SSTSIM_X_LDADD} "${LIBZ_LIBS}")
    set(SSTINFO_X_LDADD ${SSTINFO_X_LDADD} "${LIBZ_LIBS}")
endif()

if (USE_HDF5)
    set(AM_CPPFLAGS ${AM_CPPFLAGS} "${HDF5_CFLAGS}")
    set(SSTSIM_X_SOURCES ${SSTSIM_X_SOURCES} "statapi/statoutputhdf5.cc")
    set(SSTINFO_X_SOURCES ${SSTINFO_X_SOURCES} "statapi/statoutputhdf5.cc")
    set(SSTSIM_X_LDADD ${SSTSIM_X_LDADD} "${HDF5_LDFLAGS} ${HDF5_LIBS}")
    set(SSTINFO_X_LDADD ${SSTINFO_X_LDADD} "${HDF5_LDFLAGS} ${HDF5_LIBS}")
endif()
