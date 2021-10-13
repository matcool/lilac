cmake_minimum_required(VERSION 3.8)

add_library(loader_test SHARED main.cpp)
target_link_libraries(loader_test lilac_loader)
target_include_directories(loader_test PRIVATE ../include)