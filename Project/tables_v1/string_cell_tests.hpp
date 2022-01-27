#pragma once
#ifndef TABLES_V1_STRING_CELL_TESTS_HPP
#define TABLES_V1_STRING_CELL_TESTS_HPP
#include "catch.hpp"
#include "string_cell.hpp"

TEST_CASE("String cells are created correctly", "[string_cell]") {
    SECTION("string values are interpreted correctly") {
        string_cell c("42 hello world 42");
        REQUIRE(c.get_numeric().T == type::Int);
        REQUIRE(c.get_numeric().V.i_val == 0);
        REQUIRE(c.save_value() == "\"42 hello world 42\"");

    }
}


#endif //TABLES_V1_STRING_CELL_TESTS_HPP
