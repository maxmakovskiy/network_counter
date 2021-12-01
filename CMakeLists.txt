cmake_minimum_required(VERSION 3.8)

project(network_counter)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

#if(PRINT_PATH)
#    set(GCC_COMPILE_FLAGS "-H -fsyntax-only")
#    add_definitions(${GCC_COMPILE_FLAGS})
#endif()

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CSS_FLAGS_DEBUG "-g")
set(CMAKE_CSS_FLAGS_RELEASE "-O2")

#add_compile_definitions(VISUAL_DEBUG)

add_library(net_sniffer_lib STATIC frame.cpp manager.cpp)

add_executable(main main.cpp)
target_link_libraries(main net_sniffer_lib)

