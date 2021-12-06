cmake_minimum_required(VERSION 3.8)

project(network_counter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
    
option(PRINT_HEADER_PATH "Debug or Realese?" OFF)
option(PRINT_VISUAL_DEBUG "Debug or Realese?" OFF)
option(THROW_ERRORS_IN_SQL_QUERIES "Debug or Realese?" OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(PRINT_HEADER_PATH)
    set(GCC_COMPILE_FLAGS "-H -fsyntax-only")
    add_definitions(${GCC_COMPILE_FLAGS})
endif()

if(THROW_ERRORS_IN_SQL_QUERIES)
    add_compile_definitions(SQL_DEBUG_EXCEPTION)
endif()

if(PRINT_VISUAL_DEBUG)
    add_compile_definitions(VISUAL_DEBUG)
endif()

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CSS_FLAGS_DEBUG "-g")
set(CMAKE_CSS_FLAGS_RELEASE "-O2")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(net_sniffer_lib STATIC src/frame.cpp src/manager.cpp src/ntfile.cpp src/unpacked_frame.cpp src/sqlite_dao.cpp)

add_executable(main src/main.cpp)

target_link_libraries(main net_sniffer_lib ${CONAN_LIBS})

