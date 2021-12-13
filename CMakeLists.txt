cmake_minimum_required(VERSION 3.11)

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

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)
 
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG cecd54df42dd66fdf8386ed461e16b725bffc827
)
 
FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
# --- FTXUI end ----------------------------------------------------------------

# --- Conan --------------------------------------------------------------------
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
# --- Conan end ----------------------------------------------------------------

add_library(net_sniffer_lib STATIC 
    src/frame.cpp
    src/manager.cpp
    src/ntfile.cpp
    src/unpacked_frame.cpp
    src/sqlite_dao.cpp
)

add_executable(main src/main.cpp)

target_include_directories(main PRIVATE src)

target_link_libraries(main
    net_sniffer_lib 
    ${CONAN_LIBS}
    ftxui::screen
    ftxui::dom
)

