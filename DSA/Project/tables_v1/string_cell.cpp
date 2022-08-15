#include "string_cell.hpp"

string_cell::string_cell(const std::string& str)
        : val(str) {
    type T = determine_type(str.c_str());
    num_value.T = T;
    switch (T) {
        case type::String: {
            num_value.V.i_val = 0;
            break;
        }
        case type::Int: {
            num_value.V.i_val = std::stoi(str);
            break;
        }
        case type::Double: {
            num_value.V.d_val = std::stod(str);
            break;
        }
    }
}

numeric_value string_cell::get_numeric() const {
    return num_value.T == type::String ?
           numeric_value{ type::Int, num_value.V } :
           num_value;
}

std::string string_cell::save_value() const {
    std::string res;
    const char* current = val.c_str();
    res.push_back('\"');
    while(*current) {
        while(*current && *current != '\\' && *current != '\"') {
            res.push_back(*current);
            ++current;
        }
        if(*current == '\"') {
            res.push_back('\\');
            res.push_back('\"');
        }
        if(*current == '\\') {
            res.push_back('\\');
            res.push_back('\\');
        }
        if(*current) ++current;
    }
    res.push_back('\"');
    return res;
}

string_cell* string_cell::clone() const {
    return new string_cell(*this);
}

std::string string_cell::print_value() const {
    return get_numeric().print_value();
}

void string_cell::increment_value() {
    if(num_value.T == type::Int) {
        num_value.increment();
        val = std::to_string(num_value.V.i_val);
    }
}

void string_cell::decrement_value() {
    if(num_value.T == type::Int) {
        num_value.decrement();
        val = std::to_string(num_value.V.i_val);
    }
}

type string_cell::get_type() const {
    return num_value.T;
}

std::string string_cell::print_expr() const {
    return val;
}
