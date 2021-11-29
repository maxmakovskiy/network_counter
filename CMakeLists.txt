cmake_minimum_required(VERSION 3.8)

project(network_counter)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CSS_FLAGS_DEBUG "-g")
set(CMAKE_CSS_FLAGS_RELEASE "-O2")

add_library(net_sniffer_lib STATIC frame.cpp manager.cpp)

add_executable(main main.cpp frame.cpp manager.cpp)
target_link_libraries(main net_sniffer_lib)

