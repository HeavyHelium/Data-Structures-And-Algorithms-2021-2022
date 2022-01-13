cmake_minimum_required(VERSION 3.21)
project(hw3_2)

set(CMAKE_CXX_STANDARD 17)

add_executable(console console.cpp implementation.cpp input_output_handler.cpp)
add_executable(tests test.cpp implementation.cpp)
