cmake_minimum_required(VERSION 3.21)
project(hash)

set(CMAKE_CXX_STANDARD 17)

add_executable(hash main.cpp implementation.cpp interface.h data_parser.cpp data_parser.h string_operations.cpp string_operations.h)
