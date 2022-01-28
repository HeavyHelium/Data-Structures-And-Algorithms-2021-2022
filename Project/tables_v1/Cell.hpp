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
    virtual numeric_value get_numeric() const = 0;
    virtual std::string save_value() const = 0;
    virtual std::string print_value() const = 0;
    virtual std::string print_expr() const = 0;
    virtual base_cell* clone() const = 0;
    virtual ~base_cell() = default;
};

/// in case a clear cell operation is added, for now it's not meant to be used
class empty_cell : public base_cell {
public:
    empty_cell() = default;
    numeric_value get_numeric() const;
};

/// a factory function
base_cell* make_cell(const std::string& value, const absolute_cellname& name, table& t);

#endif //TABLES_V1_CELL_HPP
