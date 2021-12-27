cmake_minimum_required(VERSION 3.21)
project(hw2_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(console console.cpp data_parser.h command_parser.h string_operations.h input_handler.h implementation.cpp string_operations.cpp input_handler.cpp data_parser.cpp command_parser.cpp)
add_executable(tests tests.cpp implementation.cpp string_operations.cpp my_tests.h data_parser.cpp)
