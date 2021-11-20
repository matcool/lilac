cmake_minimum_required(VERSION 3.8)

project(lilac_loader LANGUAGES CXX VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES 
  src/*.cpp
)

add_library(lilac_loader SHARED ${SOURCES})

add_subdirectory(submodules/meta)
add_subdirectory(submodules/core)
include("submodules/lib/include.cmake")

include_directories(
  "${CMAKE_SOURCE_DIR}/api"
  "${CMAKE_SOURCE_DIR}/api/lilac"
  "${CMAKE_SOURCE_DIR}/api/utils"
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/src/lilac"
)

add_compile_definitions(_EXPORTING)
add_compile_definitions(LILAC_PLATFORM_CONSOLE)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
