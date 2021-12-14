cmake_minimum_required(VERSION 3.3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME TestMod2)

project(${PROJECT_NAME} VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED mod2.cpp)

remove_definitions(-D_EXPORTING_LILAC_MOD)

create_lilac_file(${PROJECT_NAME})

set(WIN32 ON)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" LINK_FLAGS "/DELAYLOAD:TestMod1.dll")

target_link_libraries(
    ${PROJECT_NAME}
    lilac_loader
    TestMod1
)

