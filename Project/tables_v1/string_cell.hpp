#pragma once
#ifndef TABLES_V1_STRING_CELL_HPP
#define TABLES_V1_STRING_CELL_HPP
#include "string_cell.hpp"
#include "numeric_value.hpp"
#include "Cell.hpp"
#include <string>

class string_cell : public base_cell {
    std::string val;
    numeric_value num_value;
public:
    string_cell(const std::string& str);
    numeric_value get_numeric() const override;
    std::string save_value() const override;
    std::string print_value() const override;
    std::string print_expr() const override;
    type get_type() const;
    void increment_value();
    void decrement_value();
    string_cell* clone() const override;
};


#endif //TABLES_V1_STRING_CELL_HPP
