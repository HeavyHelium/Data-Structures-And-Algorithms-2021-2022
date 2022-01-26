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
    REQUIRE(t1.get_value({ 1, 1 }) == "hello world!");
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

#endif //TABLES_V1_TABLE_TESTS_HPP
