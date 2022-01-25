#pragma once
#ifndef TABLES_V1_EXPRESSION_TOKENIZER_HPP
#define TABLES_V1_EXPRESSION_TOKENIZER_HPP
#include <string>
#include <vector>
#include "Cell.hpp"
#include <unordered_map>
#include "expression_token.h"

struct operator_info {
    const char* val;
    operator_type t;
};

class expression_tokenizer {
    std::vector<base_token*> tokens;
    static inline const std::unordered_multimap<const char*, operator_token> operators = {
       { "*",  operator_token(operator_type::Multiply,  3, true) },
       { "+",  operator_token(operator_type::Plus,      2, true) },
       { "-",  operator_token(operator_type::Minus,     2, true) },
       { "-",  operator_token(operator_type::U_minus,   4, true) },
       { "+",  operator_token(operator_type::U_plus,    4, true) },
       { "//", operator_token(operator_type::Div,       3, true) },
       { "==", operator_token(operator_type::Equals,    0, true) },
       { "!=", operator_token(operator_type::NotEquals, 0, true) },
       { ">",  operator_token(operator_type::Smaller,   1, true) },
       { "<",  operator_token(operator_type::Greater,   1, true) },
     };
    static inline const std::unordered_multimap<const char*, function_token> functions = {

    };

    static inline const char* functions[] = { "if", "and", "not", "or", "sum", "count" };
public:
    expression_tokenizer() = default;
    void parse(const std::string& text);
    std::vector<std::string> split(const std::string& text);
};


#endif //TABLES_V1_EXPRESSION_TOKENIZER_HPP
