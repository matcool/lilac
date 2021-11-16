cmake_minimum_required(VERSION 3.8)

project(lilac_loader LANGUAGES CXX VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES 
  src/*.cpp
)

add_library(lilac_loader SHARED ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

add_subdirectory(submodules/meta)
add_subdirectory(submodules/src)
include("submodules/lib/include.cmake")

include_directories(
  "${CMAKE_SOURCE_DIR}/api/lilac"
)

add_compile_definitions(_EXPORTING)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
