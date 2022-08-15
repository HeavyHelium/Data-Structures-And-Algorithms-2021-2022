#include "Cell.hpp"
#include "table.hpp"
#include "string_helpers.hpp"
#include "string_cell.hpp"
#include "expression_cell.hpp"

/// factory function
base_cell* make_cell(const std::string& value, const absolute_cellname& name, table& t) {
    const char* current = value.c_str();
    valid_value v = valid_string(current);
    if(v.valid) return new string_cell(v.value);
    return new expression_cell(name, t, value);
}

