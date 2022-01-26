#pragma once
#ifndef TABLES_V1_TABLE_HPP
#define TABLES_V1_TABLE_HPP
#include <unordered_map>
#include "cellname.hpp"
#include <utility>
#include <map>

class base_cell;
class numeric_value;

using coordinates = std::pair<int, int>;
/// Matrix is represented as a dictionary of keys
using cell_map = std::map<std::pair<int, int>, base_cell*>;
using table_iterator = cell_map::iterator;
using const_table_iterator = cell_map::const_iterator;


enum cell_type {
    empty,
    str,
    expr,
};

class table {
    cell_map t;
    int max_column = 0;
    int max_row = 0;
    int cell_count = 0;
public:
    /// Rule of 3
    table(const table& other);
    table& operator=(const table& other);
    ~table();
    table() = default;
    void set(const absolute_cellname& n, const std::string& value);
    std::string get_value(const absolute_cellname& n) const;
    cell_type get_type(const absolute_cellname& n) const;
    numeric_value get_num_value(const absolute_cellname& n) const;
    void print_val(const absolute_cellname& address) const;
    void print_expr(const absolute_cellname& address) const;
    void print_val_all(const absolute_cellname& address) const;
    void print_expr_all(const absolute_cellname& address) const;
    void save(const std::string& ofilename) const;
    void load(const std::string& ifilename);
    void incr(const absolute_cellname& address);
    void dcr(const absolute_cellname& address);
    numeric_value sum_area(const absolute_cellname& address1, const absolute_cellname& address2) const;
    int count_area(const absolute_cellname& address1, const absolute_cellname& address2) const;
    bool valid_address(const std::string& address) const;
    const_table_iterator find_cell(const absolute_cellname& n) const;
    int count() const;
private:
    void validate_address(const absolute_cellname& address) const;
    void clear();
};


#endif //TABLES_V1_TABLE_HPP
