#include "Cell.hpp"
#include "string_helpers.hpp"

class table;

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

expression_cell::expression_cell(const absolute_cellname& name, const std::string& text)
    : name(name) {
    // TODO: parse text
}
bool expression_cell::contains(const absolute_cellname& name) {
    return false;
}

base_cell* make_cell(const std::string& value, const cellname& name, table& t) {
    const char* current = value.c_str();
    valid_value v = valid_string(current);
    if(v.valid) return new string_cell(v.value);
    return new expression_cell();
}