#include "expression_token.h"

base_token::base_token(const int precedence, const bool left_associative)
        : precedence(precedence), left_associative(left_associative)
{}

operator_token::operator_token(operator_type t, const int precedence, const bool left_associative)
        : t(t), base_token(precedence, left_associative)
{}

std::string operator_token::save_value() const {
    return std::string();
}

operator_token* operator_token::clone() const {
    return new operator_token(*this);
}

std::string function_token::save_value() const {
    return std::string();
}

function_token::function_token(const std::string &value, const int precedence, const bool left_associative)
    : base_token(precedence, left_associative) {

}

function_token* function_token::clone() const {
    return new function_token(*this);
}

std::string int_token::save_value() const {
    return std::string();
}

int_token::int_token(int value) : value(value) {

}

int_token* int_token::clone() const {
    return new int_token(*this);
}

std::string double_token::save_value() const {
    return std::string();
}

double_token::double_token(double value) : value(value) {

}

double_token* double_token::clone() const {
    return new double_token(*this);
}
