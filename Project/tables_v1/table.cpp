#include "table.hpp"
#include "string_helpers.hpp"
#include "Cell.hpp"
#include "expression_cell.hpp"
#include "string_cell.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <utility>
#include <typeinfo>

void table::set(const absolute_cellname& n, const std::string& value) {
    base_cell* new_cell = nullptr;
    try {
        new_cell = make_cell(value, n, *this);
        table_iterator found = t.find({ n.row(), n.column() });
        if(found != t.end()) {
            delete found->second;
            found->second = new_cell;
        }
        else t.insert({ { n.row(), n.column() }, new_cell });
        if(n.row() > max_row) {
            max_row = n.row();
        }
        if(n.column() > max_column) {
            max_column = n.column();
        }
        ++cell_count;
        update_table(n);
    } catch(...) {
        delete new_cell;
        throw;
    }
}

void table::print_val(const absolute_cellname& address) const {
    validate_address(address);
    const_table_iterator found = t.find({ address.row(), address.column() });
    if(found == t.end()) {
        std::cout << "empty\n";
    } else {
        std::cout << found->second->print_value() << "\n";
    }
}

void table::print_expr(const absolute_cellname& address) const {
    validate_address(address);
    const_table_iterator found = t.find({ address.row(), address.column() });
    if(found == t.end()) {
        std::cout << "empty\n";
    } else {
        std::cout << found->second->print_expr() << "\n";
    }
}

void table::print_val_all() const {
    //TODO: implement
}

void table::print_expr_all() const {
    //TODO: implement
}

void table::save(const std::string& ofilename) const {
    std::ofstream ofile(ofilename.c_str());
    if(!ofile.is_open()) {
        throw std::runtime_error("failed to open file");
    }
    for(int i = 0; i <= max_row; ++i) {
        for(int j = 0; j <= max_column; ++j) {
            const_table_iterator found = t.find({ i, j });
            if(found == t.end()) {
                ofile << ", ";
            } else {
                ofile << found->second->save_value() << ", ";
            }
        }
        ofile << '\n';
    }
    std::cout << "successfully saved table in " << ofilename << std::endl;
}

void table::load(const std::string& ifilename) {
    try {
        std::ifstream ifile(ifilename.c_str());
        if (!ifile.is_open()) {
            throw std::runtime_error("failed to open file");
        }
        int current_line = -1;
        std::string line;
        while (std::getline(ifile, line)) {
            ++current_line;
            int current_col = -1;
            const char *l = line.c_str();
            while (*l) {
                std::string cell_info;
                skip_white_space(l);
                while (*l && *l != ',') {
                    cell_info.push_back(*(l++));
                }
                if (*l == ',') {
                    ++current_col;
                    ++l;
                }
                skip_white_space(l);
                if (!cell_info.empty() && *l && *l != ',') {
                    std::cout << l;
                    throw std::runtime_error("on line " + std::to_string(current_line) + ": invalid format");
                }
                if (!cell_info.empty()) {
                    set({current_line, current_col}, cell_info);
                }
            }
        }
    } catch(...) {
        clear();
        throw ;
    }
}

void table::incr(const absolute_cellname& address) {
    validate_address(address);
    table_iterator found = t.find({ address.row(), address.column() });
    if(found == t.end()) {
        throw std::invalid_argument("cannot increment an empty cell");
    } else if(typeid(*found->second) == typeid(expression_cell)) {
        numeric_value val = found->second->get_numeric();
        if(val.T == type::Int) {
            val.increment();
            base_cell* new_cell = new string_cell(std::to_string(val.V.i_val));
            delete found->second;
            found->second = new_cell;
            update_table(address);
            return;
        }
    } else {
        string_cell* temp = dynamic_cast<string_cell*>(found->second);
        if(temp->get_type() == type::Int) {
            temp->increment_value();
            update_table(address);
            return;
        }
        throw std::invalid_argument("cannot increment cell");
    }
}

void table::dcr(const absolute_cellname& address) {
    validate_address(address);
    validate_address(address);
    table_iterator found = t.find({ address.row(), address.column() });
    if(found == t.end()) {
        throw std::invalid_argument("cannot decrement an empty cell");
    } else if(typeid(*found->second) == typeid(expression_cell)) {
        numeric_value val = found->second->get_numeric();
        if(val.T == type::Int) val.decrement();
        base_cell* new_cell = new string_cell(std::to_string(val.V.i_val));
        delete found->second;
        found->second = new_cell;
        return;
    } else {
        if(found->second->get_numeric().T == type::Int) {
            dynamic_cast<string_cell*>(found->second)->decrement_value();
            return;
        }
        throw std::invalid_argument("cannot decrement cell");
    }
}

bool table::valid_address(const std::string& address) const {
    try {
        absolute_cellname name(address);
        if(name.column() > max_column ||
           name.row() > max_row) {
            return false;
        } else {
            return true;
        }
    }
    catch(...) {
        return false;
    }
}

void table::validate_address(const absolute_cellname& address) const {
    if(address.column() > max_column ||
       address.row() > max_row) {
        throw std::invalid_argument("coordinates out of bound");
    }
}

int table::count() const {
    return cell_count;
}

std::string table::get_value(const absolute_cellname &n) const {
    validate_address(n);
    const_table_iterator found = t.find({ n.row(), n.column() });
    if(found == t.end()) {
        return "";
    } else {
        return found->second->print_value();
    }
}

cell_type table::get_type(const absolute_cellname &n) const {
    validate_address(n);
    const_table_iterator found = t.find({ n.row(), n.column() });
    if(found == t.end()) {
        return cell_type::empty;
    } else if(typeid(*found->second) == typeid(string_cell)) {
        return cell_type::str;
    } else {
        return cell_type::expr;
    }
}

numeric_value table::get_num_value(const absolute_cellname &n) const {
    validate_address(n);
    const_table_iterator found = t.find({ n.row(), n.column() });
    if(found == t.end()) {
        numeric_value res{ type::Int };
        res.V.i_val = 0;
        return res;
    } else {
        return found->second->get_numeric();
    }
}

numeric_value table::sum_area(const absolute_cellname& address1, const absolute_cellname& address2) const {
    validate_address(address1);
    validate_address(address2);
    if(address1.row() > address2.row() ||
       address1.column() > address2.row()) {
        throw std::invalid_argument("invalid cell area corners");
    }
    numeric_value val{ type::Int };
    val.V.i_val = 0;
    for(int i = address1.row(); i <= address2.row(); ++i) {
        for(int j = address1.column(); j <= address2.column(); ++j) {
            const_table_iterator found = t.find({ i, j });
            if(found != t.end()) {
                val += found->second->get_numeric();
            }
        }
    }
    return val;
}

int table::count_area(const absolute_cellname &address1, const absolute_cellname &address2) const {
    validate_address(address1);
    validate_address(address2);
    if(address1.row() > address2.row() ||
       address1.column() > address2.row()) {
        throw std::invalid_argument("invalid cell area corners");
    }
    int cnt = 0;
    for(int i = address1.row(); i <= address2.row(); ++i) {
        for(int j = address1.column(); j <= address2.column(); ++j) {
            const_table_iterator found = t.find({ i, j });
            if(found != t.end()) {
                ++cnt;
            }
        }
    }
    return cnt;
}

const_table_iterator table::find_cell(const absolute_cellname &n) const {
    return t.find({ n.row(), n.column() });
}

bool is_expression(const base_cell* cell) {
    return typeid(*cell) == typeid(expression_cell);
}

void table::update_table(const absolute_cellname& current) {
    for(table_iterator iter = t.begin(); iter != t.end(); ++iter) {
        if(is_expression(iter->second) &&
           current.row() != iter->first.row() &&
           current.column() != iter->first.column()) {
           expression_cell* temp = dynamic_cast<expression_cell*>(iter->second);
           temp->evaluate(*this);
        }
    }
}

void table::clear() {
    free_cell_map(t);
    max_row = max_column = 0;
}
/// Rule of 3
table::table(const table& other)
        : max_row(other.max_row),
          max_column(other.max_column),
          cell_count(other.cell_count) {
    try {
        for(const std::pair<absolute_cellname, base_cell*>& pair : other.t) {
            t.insert({ pair.first,
                          pair.second->clone() });
        }
    } catch(...) {
        free_cell_map(t);
        throw ;
    }
}

table& table::operator=(const table &other) {
    if(this != &other) {
        cell_map temp;
        try {
            for(const std::pair<absolute_cellname, base_cell*>& pair : other.t) {
                temp.insert({ pair.first,
                              pair.second->clone() });
            }
        } catch(...) {
            free_cell_map(temp);
            throw ;
        }
        clear();
        t = std::move(temp);
        max_row = other.max_row;
        max_column = other.max_column;
        cell_count = other.cell_count;
    }
    return *this;
}

table::~table() {
    clear();
}

void table::free_cell_map(cell_map& m) {
    for(const std::pair<absolute_cellname, base_cell*>& pair : m) {
        delete pair.second;
    }
}



