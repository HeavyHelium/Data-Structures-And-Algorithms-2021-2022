#pragma once
#ifndef TABLES_V1_TABLE_HPP
#define TABLES_V1_TABLE_HPP
#include <unordered_map>
#include "cellname.hpp"

class base_cell;

using table_iterator = std::unordered_multimap<absolute_cellname, base_cell*>::iterator;
using const_table_iterator = std::unordered_multimap<absolute_cellname, base_cell*>::const_iterator;

class table {
    std::unordered_multimap<absolute_cellname, base_cell*> t;
    int max_column = 0;
    int max_row = 0;
public:
    void set(const absolute_cellname& n, const std::string& value);
    void print_val(const absolute_cellname& address) const;
    void print_expr(const absolute_cellname& address) const;
    void print_val_all(const absolute_cellname& address) const;
    void print_expr_all(const absolute_cellname& address) const;
    void save(const std::string& ofilename) const;
    void load(const std::string& ifilename);
    void incr(const cellname& address);
    void dcr(const cellname& address);
};


#endif //TABLES_V1_TABLE_HPP
