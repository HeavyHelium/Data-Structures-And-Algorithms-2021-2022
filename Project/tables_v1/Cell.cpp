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
    return val;
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

expression_cell::expression_cell(const absolute_cellname &name, table& table_link, const std::string& text)
    : name(name) {
    evaluate(table_link);
}

expression_cell::~expression_cell() {
    for(base_token* token : tokens) {
        delete token;
    }
}

std::string expression_cell::save_value() const {
    std::string res;
    // TODO: implement
    return res;
}

bool expression_cell::contains(const absolute_cellname& name) const {
    for(base_token* ptr : tokens) {
        if(typeid(*ptr) == typeid(absolute_cellname_token)) {
            const absolute_cellname_token* temp = dynamic_cast<const absolute_cellname_token*>(ptr);
            if(name.row() == temp->name.row() &&
               name.column() == temp->name.column()) {
                return true;
            }
        } else if(typeid(*ptr) == typeid(relative_cellname)) {
            const relative_cellname_token* temp = dynamic_cast<const relative_cellname_token*>(ptr);
            int actual_r = temp->name.row() + this->name.row();
            int actual_c = temp->name.column() + this->name.column();
            if(actual_r == name.row() &&
               actual_c == name.column()) {
                return true;
            }
        }
    }
    return false;
}

expression_cell::expression_cell(const expression_cell& other)
    : name(other.name),
      val(other.val),
      tokens(copy_tokens(other.tokens))
{}

expression_cell& expression_cell::operator=(const expression_cell &other) {
    if(this != & other) {
        std::vector<base_token *> new_tokens(copy_tokens(other.tokens));
        free_tokens(tokens);
        val = other.val;
        name = other.name;
        tokens = std::move(new_tokens);
    }
    return *this;
}

std::vector<base_token *> expression_cell::copy_tokens(const std::vector<base_token*> tokens) {
    std::vector<base_token*> result;
    for(const base_token* ptr : tokens) {
        result.push_back(ptr->clone());
    }
    return result;
}

void expression_cell::free_tokens(std::vector<base_token *> tokens) {
    for(base_token* ptr : tokens) {
        delete ptr;
    }
}

numeric_value expression_cell::get_numeric() const {
    return val;
}

expression_cell* expression_cell::clone() const {
    return new expression_cell(*this);
}

/// to prevent memory leak
void expression_cell::evaluate(table &table_link) {
    free_tokens(tokens);
}

std::string expression_cell::print_value() const {
    return val.print_value();
}
/// factory function
base_cell* make_cell(const std::string& value, const absolute_cellname& name, table& t) {
    const char* current = value.c_str();
    valid_value v = valid_string(current);
    if(v.valid) return new string_cell(v.value);
    return new expression_cell(name, t, value);
}

