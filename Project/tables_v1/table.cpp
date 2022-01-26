#include <stdexcept>
#include "table.hpp"
#include "string_helpers.hpp"
#include "Cell.hpp"
#include <iostream>
#include <fstream>

//second is the base_cell*
void table::set(const absolute_cellname& n, const std::string& value) {
    base_cell* new_cell = make_cell(value, n, *this);
    auto found = t.find({ n.row(), n.column() });
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
}

void table::print_val(const absolute_cellname& address) const {
    validate_address(address);
    auto found = t.find({ address.row(), address.column() });
    if(found == t.end()) {
        std::cout << "empty\n";
    } else {
        std::cout << found->second->print_value() << "\n";
    }
}

void table::print_expr(const absolute_cellname& address) const {
    validate_address(address);
    //TODO: implement
    std::cout << "expression\n";
}

void table::print_val_all(const absolute_cellname& address) const {
    validate_address(address);
    //TODO: implement
}

void table::print_expr_all(const absolute_cellname& address) const {
    validate_address(address);
    //TODO: implement
}

void table::save(const std::string& ofilename) const {
    //TODO: implement
}

void table::load(const std::string& ifilename) {
    //TODO: implement
}

void table::incr(const absolute_cellname& address) {
    validate_address(address);
    auto found = t.find({ address.row(), address.column() });
    if(found == t.end()) {
        throw std::invalid_argument("cannot increment an empty cell");
    } else if(typeid(*found->second) == typeid(expression_cell)) {
        numeric_value val = found->second->get_numeric();
        if(val.T == type::Int) {
            val.increment();
            base_cell* new_cell = new string_cell(std::to_string(val.V.i_val));
            delete found->second;
            found->second = new_cell;
            return;
        }
    } else {
        if(found->second->get_numeric().T == type::Int) {
            dynamic_cast<string_cell*>(found->second)->increment_value();
            return;
        }
        throw std::invalid_argument("cannot increment cell");
    }
}

void table::dcr(const absolute_cellname& address) {
    validate_address(address);
    validate_address(address);
    auto found = t.find({ address.row(), address.column() });
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

void table::clear() {
    for(auto pair : t) {
        delete pair.second;
    }
    t.clear();
    max_row = max_column = 0;
}
/// Rule of 3
table::table(const table& other)
    : max_row(other.max_row),
      max_column(other.max_column),
      cell_count(other.cell_count) {
    cell_map temp;
    for(auto pair : other.t) {
        temp.insert({ pair.first,
                      pair.second->clone() });
    }
    t = temp;
}

table& table::operator=(const table &other) {
    if(this != &other) {
        cell_map temp;
        for(auto pair : other.t) {
            temp.insert({ pair.first,
                          pair.second->clone() });
        }
        clear();
        max_row = other.max_row;
        max_column = other.max_column;
        cell_count = other.cell_count;
    }
    return *this;
}

table::~table() {
    clear();
}

int table::count() const {
    return cell_count;
}

std::string table::get_value(const absolute_cellname &n) const {
    validate_address(n);
    auto found = t.find({ n.row(), n.column() });
    if(found == t.end()) {
        return "";
    } else {
        return found->second->print_value();
    }
}

cell_type table::get_type(const absolute_cellname &n) const {
    validate_address(n);
    auto found = t.find({ n.row(), n.column() });
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
    auto found = t.find({ n.row(), n.column() });
    if(found == t.end()) {
        throw std::invalid_argument("cell is empty");
    } else {
        return found->second->get_numeric();
    }
}
