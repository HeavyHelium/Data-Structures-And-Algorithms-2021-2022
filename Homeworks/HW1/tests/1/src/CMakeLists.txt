cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(tests)

set(SOURCES
	interface.h
	tests.cpp
	implementation.hpp
	implementation.cpp
	list.hpp
	list_with_sorted_insertion.hpp
	queue.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ../../../group-G/source/common)