#pragma once
#ifndef TABLES_V1_EXPRESSION_CELL_TESTS_HPP
#define TABLES_V1_EXPRESSION_CELL_TESTS_HPP
#include "expression_cell.hpp"
#include "catch.hpp"
#include <string>

TEST_CASE("Tokenization of string is correctly performed", "[expression_cell]") {
    SECTION("For simple equations consisting of integers and operators") {
        const char* expr1 = "1 + 2 + 3 / 5";
        std::vector<base_token*> tokens = expression_cell::tokenize(expr1);
        std::string res = expression_cell::tokens_to_string(tokens);
        REQUIRE(res == "1 + 2 + 3 / 5 ");
        expression_cell::free_tokens(tokens);
    }
    SECTION("For more complex expressions with unary operators") {
        const char* expr2 = "-1 + 17 *42 - 365 * 14 / 7";
        std::vector<base_token*> tokens = expression_cell::tokenize(expr2);
        std::string res = expression_cell::tokens_to_string(tokens);
        REQUIRE(res == "- 1 + 17 * 42 - 365 * 14 / 7 ");
        expression_cell::free_tokens(tokens);
    }
    SECTION("For ones including cell addresses") {
        const char* expr2 = "-1 + R15C2 *42 -   365 * 14 / 7 *  R[15]C[-5]";
        std::vector<base_token*> tokens = expression_cell::tokenize(expr2);
        std::string res = expression_cell::tokens_to_string(tokens);
        REQUIRE(res == "- 1 + R15C2 * 42 - 365 * 14 / 7 * R[15]C[-5] ");
        expression_cell::free_tokens(tokens);
    }
    SECTION("For ones including function names") {
        const char* expr2 = "-1 + ( R15C2 *42 -   365 ) * 14 / and(7 *  R[15]C[-5], 8, 14)";
        std::vector<base_token*> tokens = expression_cell::tokenize(expr2);
        std::string res = expression_cell::tokens_to_string(tokens);
        REQUIRE(res == "- 1 + ( R15C2 * 42 - 365 ) * 14 / and ( 7 * R[15]C[-5] , 8 , 14 ) ");
        expression_cell::free_tokens(tokens);
    }
}


#endif //TABLES_V1_EXPRESSION_CELL_TESTS_HPP
