#pragma once
#ifndef INS_SORTED_LST
#define INS_SORTED_LST
#include "..\..\hw1\hw1\list_with_sorted_insertion.h"
#include "catch.hpp"

TEST_CASE("Initial creation", "[list_with_sorted_insertion]")
{
    list_with_sorted_insertion<int> xs;
    REQUIRE(xs.size() == 0);
}

TEST_CASE("Insertion of elements is done correctly", "[list_with_sorted_insertion]")
{
    list_with_sorted_insertion<int> xs;
    xs.insert(5);
    xs.insert(7);
    xs.insert(3);
    xs.insert(6);
    xs.insert(2);
    xs.insert(1);
    xs.insert(4);
    typename list_with_sorted_insertion<int>::iterator iter = xs.begin();
    REQUIRE(*iter++ == 1);
    REQUIRE(*iter++ == 2);
    REQUIRE(*iter++ == 3);
    REQUIRE(*iter++ == 4);
    REQUIRE(*iter++ == 5);
    REQUIRE(*iter++ == 6);
    REQUIRE(*iter == 7);
}

TEST_CASE("Removal of elements is done correctly", "[list_with_sorted_insertion]")
{
    list_with_sorted_insertion<int> xs;
    xs.insert(5);
    xs.insert(7);
    xs.insert(3);
    xs.insert(6);
    xs.insert(2);
    xs.insert(1);
    xs.insert(4);
    xs.pop_front();
    REQUIRE(*xs.begin() == 2);
    xs.pop_front();
    REQUIRE(*xs.begin() == 3);
    xs.pop_front();
    REQUIRE(*xs.begin() == 4);
    xs.pop_front();
    REQUIRE(*xs.begin() == 5);
    xs.pop_front();
    REQUIRE(*xs.begin() == 6);
    xs.pop_front();
    REQUIRE(*xs.begin() == 7);
}

#endif
