#pragma once
#ifndef TABLES_V1_EXPRESSION_TOKEN_H
#define TABLES_V1_EXPRESSION_TOKEN_H
#include <string>
#include <vector>

struct base_token
{
    const int precedence = -1;
    const bool left_associative = false;
    base_token(const int precedence, const bool left_associative);
    base_token() = default;
    virtual std::string save_value() const = 0;
};

enum function_type {
    If,
    Sum,
    Count
};


struct function_token : base_token {
    function_type t;
    std::vector<std::string> arguments;
    function_token(const std::string& value, const int precedence = 0, const bool left_associative = true);
    std::string save_value() const override;
};

enum operator_type {
    Plus,
    U_plus,
    Minus,
    U_minus,
    Multiply,
    Div,
    Equals,
    NotEquals,
    Greater,
    Smaller,
};

struct operator_token : base_token {
    operator_type t;
    operator_token(operator_type t,
                   const int precedence = 0,
                   const bool left_associative = true);
    std::string save_value() const override;
};

struct int_token : base_token {
    const int value;
    int_token(int value);
    std::string save_value() const override;
};

struct double_token : base_token {
    const double value;
    double_token(double value);
    std::string save_value() const override;
};

#endif //TABLES_V1_EXPRESSION_TOKEN_H
