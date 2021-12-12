cmake_minimum_required(VERSION 3.8)

project(lilac_loader LANGUAGES CXX VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES 
	src/*.cpp
)

add_compile_definitions(LILAC_EXPORTING)

add_library(lilac_loader SHARED ${SOURCES})

set_target_properties(lilac_loader PROPERTIES PREFIX "" OUTPUT_NAME "lilac")

add_subdirectory(submodules/lib)
set(BUILD_TEST Off CACHE BOOL "Build the test program." FORCE)
add_subdirectory(submodules/zipper)

target_link_libraries(
	lilac_loader
	lilac_core_hook
	lilac_lib
)

if(DEBUG)
	target_link_libraries(lilac_loader staticZipperd)
else()
	target_link_libraries(lilac_loader staticZipper)
endif()

target_include_directories(
	lilac_loader PUBLIC
	"${CMAKE_SOURCE_DIR}/api"
	"${CMAKE_SOURCE_DIR}/api/lilac"
	"${CMAKE_SOURCE_DIR}/api/utils"
	"${CMAKE_SOURCE_DIR}/api/keybinds"
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/src/lilac"
	"${CMAKE_SOURCE_DIR}/src/lilac/internal"
	"${CMAKE_SOURCE_DIR}/submodules/zipper/zipper"
	"${CMAKE_SOURCE_DIR}/submodules/json"
)

add_compile_definitions(_EXPORTING)
add_compile_definitions(LILAC_PLATFORM_CONSOLE)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_subdirectory(test)
