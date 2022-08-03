cmake_minimum_required(VERSION 3.24)

set(PROJECT_NAME SMB3_Memory_Game)
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build Type" FORCE)
endif()

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Release Debug)

set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_CXX_FLAGS "-Wall -pedantic")

file(GLOB_RECURSE
	SOURCES CONFIGURE_DEPENDS
	"src/*.hpp"
	"src/*.cpp")

find_package(SFML 2.5.1 REQUIRED COMPONENTS audio graphics window system)
unset(SFML_DOC_DIR CACHE)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(
	${PROJECT_NAME} PRIVATE
	"include")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_LIST_DIR}/data
	${CMAKE_CURRENT_BINARY_DIR}/data)

target_link_libraries(${PROJECT_NAME}
	sfml-system
	sfml-window
	sfml-graphics
	sfml-audio)
