cmake_minimum_required(VERSION 3.21)
project(tables_v1)

set(CMAKE_CXX_STANDARD 17)

add_executable(tables_v1 console.cpp string_helpers.cpp input_handler.cpp command_handler.cpp Cell.cpp expression_lexer.cpp cellname.cpp table.cpp)
add_executable(tests tests.cpp string_helpers.cpp Cell.cpp expression_lexer.cpp cellname.cpp table.cpp)
