#pragma once
#ifndef TABLES_V1_CELL_HPP
#define TABLES_V1_CELL_HPP

#include <string>
#include "string_helpers.hpp"
#include "cellname.hpp"
#include <unordered_set>
#include "expression_token.h"
#include "numeric_value.hpp"

class table;

class base_cell {
public:
    ///@return the numeric value of a cell
    virtual numeric_value get_numeric() const = 0;
    ///@return the value used for saving value when saving the table
    virtual std::string save_value() const = 0;
    ///@return the stringified numeric value
    virtual std::string print_value() const = 0;
    ///@return the actual value held, without "s for strings
    ///and for expressions the expression itself
    virtual std::string print_expr() const = 0;
    ///@brief to allow for easy copy-construction of the table
    virtual base_cell* clone() const = 0;
    ///@brief a dtor
    virtual ~base_cell() = default;
};
/// a factory function
base_cell* make_cell(const std::string& value, const absolute_cellname& name, table& t);

#endif //TABLES_V1_CELL_HPP
