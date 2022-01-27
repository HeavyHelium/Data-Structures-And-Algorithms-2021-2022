cmake_minimum_required(VERSION 3.21)
project(tables_v1)

set(CMAKE_CXX_STANDARD 17)

add_executable(tables_v1 console.cpp string_helpers.cpp input_handler.cpp command_handler.cpp Cell.cpp cellname.cpp table.cpp expression_token.cpp numeric_value.cpp numeric_value.hpp expression_cell.cpp expression_cell.hpp string_cell.cpp string_cell.hpp)
add_executable(tests tests.cpp string_helpers.cpp Cell.cpp cellname.cpp table.cpp expression_token.cpp table_tests.hpp numeric_value_tests.hpp numeric_value.cpp numeric_value.hpp expression_cell.cpp expression_cell.hpp string_cell.cpp string_cell.hpp expression_cell_tests.hpp)
