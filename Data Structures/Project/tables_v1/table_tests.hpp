#pragma once
#ifndef TABLES_V1_TABLE_TESTS_HPP
#define TABLES_V1_TABLE_TESTS_HPP
#include "catch.hpp"
#include "table.hpp"
#include "Cell.hpp"

TEST_CASE("set cell works correctly", "[table]") {
    table t1;
    t1.set({ 1, 1 }, "\"hello world!\"");
    REQUIRE(t1.get_type({ 1, 1 }) == cell_type::str);
    REQUIRE(t1.get_value({ 1, 1 }) == "0");
}

TEST_CASE("incr works correctly", "[table]") {
    table t1;
    t1.set({ 1, 20 }, "\"42\"");
    REQUIRE_NOTHROW(t1.incr({ 1, 20 }));
    REQUIRE(t1.get_value({ 1, 20 }) == "43");
    t1.set({ 1, 20 }, "\"what doesn't kill you makes you stronger\"");
    REQUIRE(t1.get_type({ 1, 20 }) == cell_type::str);
    REQUIRE_THROWS(t1.incr({ 1, 20 }));
}

TEST_CASE("dcr works correctly", "[table]") {
    table t1;
    t1.set({ 1, 20 }, "\"42\"");
    REQUIRE_NOTHROW(t1.dcr({ 1, 20 }));
    REQUIRE(t1.get_value({ 1, 20 }) == "41");
    t1.set({ 1, 20 }, "\"what doesn't kill you makes you stronger\"");
    REQUIRE(t1.get_type({ 1, 20 }) == cell_type::str);
    REQUIRE_THROWS(t1.incr({ 1, 20 }));
}

TEST_CASE("expression cells evaluate expressions correctly", "[table]") {
    table t1;
    SECTION("for simple expressions with integer operands") {
        REQUIRE_NOTHROW(t1.set({ 1, 1 }, "5 + 8 + 2 * 42 / 7"));
        REQUIRE_NOTHROW(t1.set({ 1, 2 }, "5 + 8 + 2 * 42 / 7 - -16 * +4 "));
        REQUIRE_NOTHROW(t1.set({ 1, 3 }, "5 + (8 + 2) * 42"));
        REQUIRE_NOTHROW(t1.set({ 1, 4 }, "if(-5 > 16, 14, 1) + 14"));
        REQUIRE_NOTHROW(t1.set({ 1, 5 }, "not(5 + 16)"));
        REQUIRE_NOTHROW(t1.set({ 1, 6 }, "not(5 - 5)"));
        REQUIRE_NOTHROW(t1.set({ 1, 7 }, "and(1, 1, 2, 3, 5, 8)"));
        REQUIRE_NOTHROW(t1.set({ 1, 8 }, "and(0, 1, 1, 2, 3, 5, 8)"));
        REQUIRE_NOTHROW(t1.set({ 1, 9 }, "or(0, 1, 1, 2, 3, 5, 8)"));
        REQUIRE_NOTHROW(t1.set({ 1, 10 }, "or(1)"));
        REQUIRE_NOTHROW(t1.set({ 1, 11 }, "or(0, 0, 0, 0)"));
        REQUIRE(t1.get_value({ 1, 1 }) == "25");
        REQUIRE(t1.get_value({ 1, 1 }) == "25");
        REQUIRE(t1.get_value({ 1, 2 }) == "89");
        REQUIRE(t1.get_value({ 1, 3 }) == "425");
        REQUIRE(t1.get_value({ 1, 4 }) == "15");
        REQUIRE(t1.get_value({ 1, 5 }) == "0");
        REQUIRE(t1.get_value({ 1, 6 }) == "1");
        REQUIRE(t1.get_value({ 1, 7 }) == "1");
        REQUIRE(t1.get_value({ 1, 8 }) == "0");
        REQUIRE(t1.get_value({ 1, 9 }) == "1");
        REQUIRE(t1.get_value({ 1, 10 }) == "1");
        REQUIRE(t1.get_value({ 1, 11 }) == "0");
        REQUIRE(t1.count() == 11);
    }
    SECTION("throws when it should") {
        REQUIRE_THROWS(t1.set({ 1, 1 }, "5 + 8 + 2 * 42 / 7)"));
        REQUIRE_THROWS(t1.set({ 1, 2 }, "5  8 + 2 * 42 / 7 - -16 * +4 "));
        REQUIRE_THROWS(t1.set({ 1, 3 }, "((5 + (8 + 2) * 42"));
        REQUIRE_THROWS(t1.set({ 1, 4 }, "5 / 0"));
        REQUIRE_THROWS(t1.set({ 1, 5 }, "not(5 + 16, 17)"));
        REQUIRE_THROWS(t1.set({ 1, 6 }, "if(5 > 7, 4, 5, 6)"));
        REQUIRE_THROWS(t1.set({ 1, 6 }, "if(unknown_token > 7, 4, 5, 6)"));
    }
    SECTION("for more complex ones containing cellnames") {
        REQUIRE_NOTHROW(t1.set({ 0, 0 }, "0"));
        REQUIRE_NOTHROW(t1.set({ 1, 1 }, "5 + R[-1]C[-1] + 2 * R0C0 / 7"));
        REQUIRE_NOTHROW(t1.set({ 1, 0 }, "\"hello world\""));
        REQUIRE_NOTHROW(t1.set({ 1, 3 }, "R1C0"));
        REQUIRE_NOTHROW(t1.set({ 1, 2 }, "5 + 8 + 2 * 42 / 7 - -R0C0 * +4 "));
        REQUIRE_NOTHROW(t1.set({ 1, 4 }, "if(R0C0 > 16, 14, if( 5 * 6 < 31, -5, -2) + 14)"));
        REQUIRE(t1.get_value({ 1, 1 }) == "5");
        REQUIRE(t1.get_value({ 1, 2 }) == "25");
        REQUIRE(t1.get_value({ 1, 3 }) == "0");
        REQUIRE(t1.get_value({ 1, 4 }) == "9");

        REQUIRE(t1.count() == 6);
    }
    SECTION("cyclic dependencies are detected") {
        REQUIRE_NOTHROW(t1.set({ 0, 0 }, "0"));
        REQUIRE_NOTHROW(t1.set({ 1, 1 }, "5 + R[-1]C[-1] + 2 * R0C0 / 7"));
        REQUIRE_NOTHROW(t1.set({ 1, 0 }, "\"hello world\""));
        REQUIRE_NOTHROW(t1.set({ 1, 3 }, "R1C0"));
        REQUIRE_NOTHROW(t1.set({ 1, 2 }, "5 + 8 + 2 * 42 / 7 - -R0C0 * +4 "));
        REQUIRE_THROWS(t1.set({ 1, 0 }, "5 + 8 + 2 * 42 / 7 - -R1C3 * +4 "));

        REQUIRE_NOTHROW(t1.set({ 1, 4 }, "if(R0C0 > 16, 14, if( 5 * 6 < 31, -5, -2) + 14)"));
        REQUIRE_THROWS(t1.set({ 1, 1 }, "R1C1 + 4"));
        REQUIRE_THROWS(t1.set({ 1, 1 }, "R[0]C[0] + 4"));
    }
}

#endif //TABLES_V1_TABLE_TESTS_HPP
