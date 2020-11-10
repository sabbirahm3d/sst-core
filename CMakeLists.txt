cmake_minimum_required(VERSION 3.12)
project(SST CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# target_include_directories(SST_CORE PUBLIC src/sst/core)
add_subdirectory(src/sst/core)
# message(STATUS ${SST_REGISTER_SOURCES})

