#pragma once
#ifndef TABLES_V1_CELLNAME_TESTS_HPP
#define TABLES_V1_CELLNAME_TESTS_HPP
#include "catch.hpp"
#include "cellname.hpp"

TEST_CASE("absolute cellnames are creared correctly") {
    REQUIRE_THROWS(absolute_cellname("aR17C5"));
    absolute_cellname c("R15C10");
    REQUIRE(c.row() == 15);
    REQUIRE(c.column() == 10);
}

TEST_CASE("relative cellnames are created correctly") {
    relative_cellname c("  R[-1 ]C[ 5  ]");
    REQUIRE(c.row() == -1);
    REQUIRE(c.column() == 5);
    REQUIRE_THROWS(relative_cellname("R[58h][-17]"));
}

#endif //TABLES_V1_CELLNAME_TESTS_HPP
