#pragma once
#ifndef TABLES_V1_TABLE_HPP
#define TABLES_V1_TABLE_HPP
#include <unordered_map>
#include "cellname.hpp"
#include <utility>
#include <functional>
#include <unordered_map>
#include <map>

class base_cell;
class numeric_value;

/// a template specialization for hashing cellnames
template<>
struct std::hash<absolute_cellname>
{
    std::size_t operator()(const absolute_cellname& name) const noexcept
    {
        std::size_t h1 = std::hash<int>{}(name.row());
        std::size_t h2 = std::hash<int>{}(name.column());
        return h1 ^ h2;
    }
};
/// Matrix is represented as a hash table with keys cell coordinates
using cell_map = std::unordered_map<absolute_cellname, base_cell*>;
using table_iterator = cell_map::iterator;
using const_table_iterator = cell_map::const_iterator;

enum cell_type {
    empty,
    str,
    expr,
};

class table {
    friend class expression_cell;
    cell_map t;
    int max_column = -1;
    int max_row = -1;
    int cell_count = 0;
public:
    /// Rule of 3
    table(const table& other);
    table& operator=(const table& other);
    ~table();
    table() = default;
    ///@brief sets a cell to a given value
    /// can be a string ot an expression
    void set(const absolute_cellname& n, const std::string& value, bool check_for_bounds = true);
    ///@brief the stringified value of a cell
    std::string get_value(const absolute_cellname& n) const;
    ///@brief gets the type of a cell
    ///@param n const absolute_cellname& n
    cell_type get_type(const absolute_cellname& n) const;
    ///@brief gets the numeric value of any cell within the table
    numeric_value get_num_value(const absolute_cellname& n) const;
    ///@brief prints the value if any cell within the table
    void print_val(const absolute_cellname& address) const;
    ///@brief prints the expression in a given cell
    void print_expr(const absolute_cellname& address) const;
    ///@brief prints the value of all cells in the table
    void print_val_all() const;
    ///@brief prints the expressions in all cells in the table
    void print_expr_all() const;
    ///@brief saves the table in a csv file
    void save(const std::string& ofilename) const;
    ///@brief reads the table from a csv file
    void load(const std::string& ifilename);
    ///@brief increments the value of a given cell representing an integer
    /// if it is an expression cell, it becomes a string cell
    /// whose value is an integer
    void incr(const absolute_cellname& address);
    ///@brief decrements the value of a given cell representing an integer
    /// if it is an expression cell, it becomes a string cell
    /// whose value is an integer
    void dcr(const absolute_cellname& address);
    ///@brief finds sum of all non-empty cells in a given area
    /// empty cells' values are treated as zeros
    numeric_value sum_area(const absolute_cellname& address1, const absolute_cellname& address2) const;
    ///@brief finds count of all non-empty cells in a given area
    int count_area(const absolute_cellname& address1, const absolute_cellname& address2) const;
    ///@brief checks whether or not a given address is within the limits of the table
    bool valid_address(const std::string& address) const;
    ///@brief finds a cell by a given address
    const_table_iterator find_cell(const absolute_cellname& n) const;
    ///@brief information method for finding the total number of non-empty cells within the table
    int count() const;
    ///@brief clears the table
    ///@brief frees cells in a cell_map
    void clear();
private:
    ///@brief validates an address
    ///@throws when address is not within the limits of the table std::invalid_argument is thrown
    void validate_address(const absolute_cellname& address) const;
    ///@brief reevaluates all expression cells which contain the current cell as part of their expression
    void update_table(const absolute_cellname& current);
    static void free_cell_map(cell_map& m);
    int number_of_digits(int number) const;
    int max_val_len_by_col(int j) const;
    int max_expr_len_by_col(int j) const;
};


#endif //TABLES_V1_TABLE_HPP
