cmake_minimum_required(VERSION 3.21)
project(hw2_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(console console.cpp data_parser.h command_parser.h string_operations.h input_handler.h)
add_executable(tests tests.cpp)
