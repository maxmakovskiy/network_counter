cmake_minimum_required(VERSION 3.8)

project(network_counter)

add_library(net_sniffer_lib STATIC frame.cpp manager.cpp)

add_executable(main main.cpp)
target_link_libraries(main net_sniffer_lib)

