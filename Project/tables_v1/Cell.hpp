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
    virtual base_cell* clone() const = 0;
    virtual ~base_cell() = default;
};


class string_cell : public base_cell {
    std::string val;
    numeric_value num_value;
public:
    string_cell(const std::string& str);
    numeric_value get_numeric() const override;
    std::string save_value() const override;
    std::string print_value() const override;
    type get_type() const;
    void increment_value();
    void decrement_value();
    string_cell* clone() const override;
};

class expression_cell : public base_cell {
    numeric_value val;
    absolute_cellname name;
    std::vector<base_token*> tokens;
public:
    /// the table link is only important when initialization occurs
    /// otherwise the expression cell represents some value(int or double) and
    expression_cell(const absolute_cellname &name, table &table_link, const std::string &text);
    bool contains(const absolute_cellname& name) const;
    void evaluate(table& table_link);
    std::string save_value() const override;
    std::string print_value() const override;
    numeric_value get_numeric() const override;
    /// to allow for virtual cloning
    expression_cell* clone() const override;

    expression_cell(const expression_cell& other);
    expression_cell& operator=(const expression_cell& other);
    ~expression_cell();
    static std::vector<base_token*> copy_tokens(const std::vector<base_token*> tokens);
    static void free_tokens(std::vector<base_token*> tokens);
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
