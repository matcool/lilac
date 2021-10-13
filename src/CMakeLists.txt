cmake_minimum_required(VERSION 3.8)

add_library(proxy_loader SHARED proxy_loader.c)
set_property(TARGET proxy_loader PROPERTY OUTPUT_NAME "XInput9_1_0")

add_library(lilac_loader SHARED impl.cpp loader_main.cpp)
set_property(TARGET lilac_loader PROPERTY CXX_STANDARD 17)
target_include_directories(lilac_loader PRIVATE ../include/core/include ../include)
target_link_libraries(lilac_loader lilac_hook)
target_compile_definitions(lilac_loader PRIVATE LILAC_EXPORT)