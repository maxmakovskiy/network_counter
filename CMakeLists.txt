cmake_minimum_required(VERSION 3.8)

project(network_counter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
    
option(PRINT_HEADER_PATH "Debug or Realese?" OFF)
option(PRINT_VISUAL_DEBUG "Debug or Realese?" OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(PRINT_HEADER_PATH)
    set(GCC_COMPILE_FLAGS "-H -fsyntax-only")
    add_definitions(${GCC_COMPILE_FLAGS})
endif()

if(PRINT_VISUAL_DEBUG)
    add_compile_definitions(VISUAL_DEBUG)
endif()

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CSS_FLAGS_DEBUG "-g")
set(CMAKE_CSS_FLAGS_RELEASE "-O2")

add_library(net_sniffer_lib STATIC frame.cpp manager.cpp ntfile.cpp)
#add_library(net_sniffer_lib STATIC frame.cpp manager.cpp)

add_executable(main main.cpp)

target_link_libraries(main net_sniffer_lib)

