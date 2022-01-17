#pragma once
#ifndef HW3_2_IMPLEMENTATION_TESTS_HPP
#define HW3_2_IMPLEMENTATION_TESTS_HPP
#include "catch.hpp"
#include "interface.h"
#include <sstream>

TEST_CASE("Comparator works correctly for the streams [] and [one two three]")
{
    std::stringstream a;
    std::stringstream b("one two three");

    Comparator c;
    ComparisonReport report = c.compare(a, b);

    SECTION("Sizes are reported correctly")
    {
        REQUIRE(report.commonWords.countOfUniqueWords() == 0);
        REQUIRE(report.uniqueWords[0].countOfUniqueWords() == 0);
        REQUIRE(report.uniqueWords[1].countOfUniqueWords() == 3);
    }

    SECTION("The second stream has unique words {one,two,three}")
    {
        std::multiset<std::string> expected{ "one","two","three" };
        REQUIRE(report.uniqueWords[1].words() == expected);
    }
}

TEST_CASE("Comparator works correctly for identical non-empty streams", "[Comparator]") {
    std::stringstream a("one two three");
    std::stringstream b("one two three");
    Comparator c;
    ComparisonReport report = c.compare(a, b);

    SECTION("Sizes are reported correctly")
    {
        REQUIRE(report.commonWords.countOfUniqueWords() == 3);
        REQUIRE(report.uniqueWords[0].countOfUniqueWords() == 0);
        REQUIRE(report.uniqueWords[1].countOfUniqueWords() == 0);
    }

    SECTION("The first stream has no unique words")
    {
        std::multiset<std::string> expected{};
        REQUIRE(report.uniqueWords[0].words() == expected);
    }
    SECTION("The second stream has no unique words")
    {
        std::multiset<std::string> expected{};
        REQUIRE(report.uniqueWords[1].words() == expected);
    }
    SECTION("The common words are the words {one, two, three}") {
        std::multiset<std::string> expected{"one", "two", "three"};
        REQUIRE(report.commonWords.words() == expected);
    }
}

#endif
