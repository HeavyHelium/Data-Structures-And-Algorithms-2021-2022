#pragma once
#ifndef TABLES_V1_NUMERIC_VALUE_TESTS_HPP
#define TABLES_V1_NUMERIC_VALUE_TESTS_HPP
#include "numeric_value.hpp"
#include "catch.hpp"

TEST_CASE("Numeric value addition works correctly", "[numeric_value]") {
    value i7;
    value d3;
    value i42;
    i7.i_val = 7;
    d3.d_val = 3.14;
    i42.i_val = 42;
    numeric_value v1{ type::Int, i7 };
    numeric_value v2{ type::Double, d3 };
    numeric_value v3{ type::Int, i42 };
    SECTION("+= between a double and an int") {
        v1 += v2;
        REQUIRE(v1.T == type::Double);
        REQUIRE(v1.V.d_val == 10.14);
    }
    SECTION("+= between an int and a double") {
        v2 += v1;
        REQUIRE(v2.T == type::Double);
        REQUIRE(v2.V.d_val == 10.14);
    }
    SECTION("+= between 2 ints") {
        v3 += v1;
        REQUIRE(v3.T == type::Int);
        REQUIRE(v3.V.i_val == 49);
    }
}

TEST_CASE("Numeric value negation works correctly", "[numeric_value]") {
    value i7;
    value d3;
    value i0;
    value i1;
    i7.i_val = 7;
    d3.d_val = 3.14;
    i0.i_val = 0;
    i1.i_val = 1;
    numeric_value v1{ type::Int, i7 };
    numeric_value v2{ type::Double, d3 };
    numeric_value v3{ type::Int, i0 };
    numeric_value v4{ type::Int, i1 };
    SECTION("boolean !") {
        REQUIRE((!v1) == v3);
        REQUIRE((!v2) == v3);
        REQUIRE((!v3) == v4);
    }
}


#endif //TABLES_V1_NUMERIC_VALUE_TESTS_HPP
