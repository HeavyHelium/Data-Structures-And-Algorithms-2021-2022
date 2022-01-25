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

std::string function_token::save_value() const {
    return std::string();
}

function_token::function_token(const std::string &value, const int precedence, const bool left_associative)
    : base_token(precedence, left_associative) {

}

std::string int_token::save_value() const {
    return std::string();
}

int_token::int_token(int value) : value(value)  {

}

std::string double_token::save_value() const {
    return std::string();
}

double_token::double_token(double value) : value(value) {

}
