#pragma once
#ifndef TABLES_V1_CELL_HPP
#define TABLES_V1_CELL_HPP

#include <string>
#include "string_helpers.hpp"
#include "cellname.hpp"
#include <unordered_map>
#include "expression_token.h"


class table;

union value {
    int i_val;
    double d_val;
};

struct numeric_value {
    type T;
    value V;
};

class base_cell {
public:
    virtual numeric_value get_numeric() const = 0;
    virtual std::string save_value() const = 0;
    virtual ~base_cell() = default;
};


class string_cell : public base_cell {
    std::string val;
    numeric_value num_value;
public:
    string_cell(const std::string& str);
    numeric_value get_numeric() const override;
    std::string save_value() const override;
};

class expression_cell : public base_cell {
    numeric_value val;
    absolute_cellname name;
    table& table_link;
    std::unordered_multimap<int, int> linked_cells;
    std::vector<base_token*> tokens;
public:
    expression_cell(const absolute_cellname &name, table &tableLink, const std::string &text);
    bool contains(const absolute_cellname& name);
    expression_cell(const expression_cell& other) = delete;
    expression_cell& operator=(const expression_cell& other) = delete;
    std::string save_value() const override;
    ~expression_cell();
};

/// in case a clear cell operation is added, for now it's not meant to be used
class empty_cell : public base_cell {
public:
    empty_cell() = default;
    numeric_value get_numeric() const;
};

base_cell* make_cell(const std::string& value);

#endif //TABLES_V1_CELL_HPP
