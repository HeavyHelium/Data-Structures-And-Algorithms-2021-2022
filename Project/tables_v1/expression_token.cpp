#include "expression_token.h"

std::string operator_token::save_value() const {
    switch(t) {
        case operator_type::Plus : {
            return "+";
        }
        case operator_type::U_plus : {
            return "+";
        }
        case operator_type::Minus : {
            return "-";
        }
        case operator_type::U_minus : {
            return "-";
        }
        case operator_type::Multiply : {
            return "*";
        }case operator_type::Div : {
            return "/";
        }
        case operator_type::Equals : {
            return "==";
        }case operator_type::NotEquals : {
            return "!=";
        }case operator_type::Smaller : {
            return "<";
        }
        case operator_type::Greater : {
            return ">";
        }
        case operator_type::L_paren : {
            return "(";
        }
        case operator_type::R_paren : {
            return ")";
        }
        case operator_type::Separator : {
            return ",";
        }
    }
}

operator_token* operator_token::clone() const {
    return new operator_token(*this);
}

operator_token::operator_token(operator_type t, const int precedence, const bool left_associative)
    : t(t), precedence(precedence), left_associative(left_associative) {
}

std::string function_token::save_value() const {
    switch(t) {
        case function_type::Sum : {
            return "sum";
        }
        case function_type::If : {
            return "if";
        }
        case function_type::Count : {
            return "count";
        }
        case function_type::And : {
            return "and";
        }
        case function_type::Not : {
            return "not";
        }
        case function_type::Or : {
            return "or";
        }
        case function_type::Sin : {
            return "sin";
        }
        case function_type::Cos : {
            return "cos";
        }
    }
}

function_token* function_token::clone() const {
    return new function_token(*this);
}

function_token::function_token(function_type t, int arg_cnt)
    : t(t), argument_count(arg_cnt) {
}

std::string int_token::save_value() const {
    return val.print_value();
}

int_token* int_token::clone() const {
    return new int_token(*this);
}

int_token::int_token(numeric_value val)
    : val(val) {
}

std::string absolute_cellname_token::save_value() const {
    return name.to_name();
}

absolute_cellname_token *absolute_cellname_token::clone() const {
    return new absolute_cellname_token(*this);
}

absolute_cellname_token::absolute_cellname_token(int r, int c)
    : name(r, c) {
}

std::string relative_cellname_token::save_value() const {
    return name.to_name();
}

relative_cellname_token *relative_cellname_token::clone() const {
    return new relative_cellname_token(*this);
}

relative_cellname_token::relative_cellname_token(int r, int c)
    : name(r, c) {
}

