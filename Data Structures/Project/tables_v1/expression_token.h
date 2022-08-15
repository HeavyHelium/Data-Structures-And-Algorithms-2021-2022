#pragma once
#ifndef TABLES_V1_EXPRESSION_TOKEN_H
#define TABLES_V1_EXPRESSION_TOKEN_H
#include <string>
#include <vector>
#include "cellname.hpp"
#include <unordered_map>
#include "numeric_value.hpp"

/// an abstract class to represent an expression token
struct base_token
{
    base_token() = default;
    ///@brief translated a token to its stringified notation
    virtual std::string save_value() const = 0;
    virtual base_token* clone() const = 0;
    virtual ~base_token() = default;
};

enum function_type {
    If,
    Sum,
    Count,
    And,
    Or,
    Not,
    Sin,
    Cos,
};


struct function_token : base_token {
    function_type t;
    int argument_count = -1;
    function_token(function_type t, int arg_cnt);
    std::string save_value() const override;
    function_token* clone() const override;
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
    L_paren,
    R_paren,
    Separator,
};

struct operator_token : base_token {
    operator_type t;
    const int precedence = -1;
    const bool left_associative = true;
    ///@brief by default all operators are left-associative
    operator_token(operator_type t,
                   const int precedence = 0,
                   const bool left_associative = true);
    std::string save_value() const override;
    operator_token* clone() const override;
};

struct int_token : base_token {
    numeric_value val;
    std::string save_value() const override;
    int_token* clone() const override;
    int_token(numeric_value val);
};

struct absolute_cellname_token : base_token {
    absolute_cellname name;
    std::string save_value() const override;
    absolute_cellname_token* clone() const override;
    absolute_cellname_token(int r, int c);
};

struct relative_cellname_token : base_token {
    relative_cellname name;
    std::string save_value() const override;
    relative_cellname_token* clone() const override;
    relative_cellname_token(int r, int c);
};


#endif //TABLES_V1_EXPRESSION_TOKEN_H
