cmake_minimum_required(VERSION 3.21)
project(tables_v1)

set(CMAKE_CXX_STANDARD 17)

add_executable(tables_v1 main.cpp string_helpers.cpp input_handler.cpp command_handler.cpp)
