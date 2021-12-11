cmake_minimum_required(VERSION 3.3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME TestMod2)

project(${PROJECT_NAME} VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED mod2.cpp)

remove_definitions(-D_EXPORTING_LILAC_MOD)

add_custom_command(
    TARGET TestMod2 POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/mod.json"
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/mod.json"
)

add_custom_command(
    TARGET TestMod2
    COMMAND ${CMAKE_COMMAND} -E tar "cf" 
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${PROJECT_NAME}.lilac" --format=zip -- 
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/mod.json"
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${PROJECT_NAME}.dll"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>"
    COMMAND ${CMAKE_COMMAND} -E echo "Creating zip file -> ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${PROJECT_NAME}.lilac"
)

set(WIN32 ON)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" LINK_FLAGS "/DELAYLOAD:TestMod2.dll")

target_link_libraries(
    ${PROJECT_NAME}
    lilac_loader
    TestMod1
)

