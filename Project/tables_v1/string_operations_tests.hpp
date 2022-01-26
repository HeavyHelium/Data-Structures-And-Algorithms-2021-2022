#pragma once
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

TEST_CASE("extraction of numbers is correctly performed") {
    const char* str1 = " 123 alabalaneshto";
    const char* temp = str1;
    string_slice res = extract_number(str1);
    string_slice expected{ temp + 1, 3 };
    REQUIRE(expected == res);
}

TEST_CASE("string slice to integer works correctly") {
    const char* str1 = " \t 123 alabalaneshto";
    string_slice res = extract_number(str1);
    REQUIRE(slice_to_i(res) == 123);
}

TEST_CASE("validity of strings is estimated correctly") {
    const char* str1 = " alabalanitsa 25 \" neshto";
    REQUIRE(!valid_string(str1).valid);
    const char* str2 = "\"yes \\\"hello world\\\" slartibartfast\"";
    valid_value v = valid_string(str2);
    REQUIRE(v.valid);
    REQUIRE(v.value == "yes \"hello world\" slartibartfast");
}

#endif //TABLES_V1_STRING_OPERATIONS_TESTS_HPP
