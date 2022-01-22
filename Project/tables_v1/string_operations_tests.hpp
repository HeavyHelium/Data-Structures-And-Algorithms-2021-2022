#ifndef TABLES_V1_STRING_OPERATIONS_TESTS_HPP
#define TABLES_V1_STRING_OPERATIONS_TESTS_HPP
#include "catch.hpp"
#include "string_helpers.hpp"

TEST_CASE("type of string is correctly determined") {
    const char* str1 = "1111.hellostr78";
    const char* str2 = "42.15";
    const char* str3 = "42";
    REQUIRE(determine_type(str1) == type::String);
    REQUIRE(determine_type(str2) == type::Double);
    REQUIRE(determine_type(str3) == type::Int);
}

#endif //TABLES_V1_STRING_OPERATIONS_TESTS_HPP
