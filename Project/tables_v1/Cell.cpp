#include "Cell.hpp"
#include "string_helpers.hpp"
#include "table.hpp"
#include <iostream>

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
    return num_value;
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

expression_cell::expression_cell(const absolute_cellname &name, table& tableLink, const std::string& text)
    : name(name), table_link(tableLink) {
    // TODO: parse text
}
bool expression_cell::contains(const absolute_cellname& name) {
    return false;
}

expression_cell::~expression_cell() {
    for(base_token* token : tokens) {
        delete token;
    }
}

std::string expression_cell::save_value() const {
    std::string res;
    return res;
}

base_cell* make_cell(const std::string& value, const cellname& name, table& t) {
    const char* current = value.c_str();
    valid_value v = valid_string(current);
    if(v.valid) return new string_cell(v.value);
    return nullptr;
}


