#include "cellname.hpp"
#include "string_helpers.hpp"
#include <stdexcept>
#include <iostream>
#include <string>

absolute_cellname::absolute_cellname(const std::string& text) {
    const char* current = text.c_str();
    skip_white_space(current);
    if(*current != 'R') {
        throw std::invalid_argument("invalid cellname");
    }
    ++current;
    skip_white_space(current);
    if(*current == '-') {
        throw std::invalid_argument("negative coordinates are not allowed!");
    }
    string_slice r = extract_number(current);
    if(r.len == 0 || *current != 'C') {
        throw std::invalid_argument("invalid cellname");
    }
    ++current;
    skip_white_space(current);
    if(*current == '-') {
        throw std::invalid_argument("negative coordinates are not allowed!");
    }
    string_slice c = extract_number(current);
    if(c.len == 0) {
        throw std::invalid_argument("invalid cellname");
    }
    if(*current) {
        throw std::invalid_argument("invalid cellname");
    }
    m_row = slice_to_i(r);
    m_column = slice_to_i(c);
}

int absolute_cellname::row() const {
    return m_row;
}

int absolute_cellname::column() const {
    return m_column;
}
///@throws when a coordinate is negative
absolute_cellname::absolute_cellname(int r, int c)
    : cellname(r, c) {
    if(r < 0 || c < 0) {
        throw std::invalid_argument("Negative coordinates are not allowed in absolute cellnames!");
    }
}

std::string absolute_cellname::to_name() const {
    return std::string("R") +
           std::to_string(m_row) +
           "C" +
           std::to_string(m_column);
}

relative_cellname::relative_cellname(const std::string &text) {
    const char* current = text.c_str();
    skip_white_space(current);
    if(*current != 'R') {
        throw std::invalid_argument("invalid cellname");
    }
    ++current;
    if(*current != '[') { throw std::invalid_argument("invalid cellname"); }
    ++current;
    skip_white_space(current);
    int sign1 = 1;
    if(*current == '-') {
        sign1 = -1;
        ++current;
    }
    skip_white_space(current);
    string_slice r = extract_number(current);
    skip_white_space(current);
    if(r.len == 0 || *current != ']') {
        throw std::invalid_argument("invalid cellname");
    }
    ++current;
    if(*current != 'C') {
        throw std::invalid_argument("invalid cellname");
    }
    ++current;
    skip_white_space(current);
    if(*current != '['){
        throw std::invalid_argument("invalid cellname");
    }
    ++current;
    skip_white_space(current);
    int sign2 = 1;
    if(*current == '-') {
        sign2 = -1;
        ++current;
    }
    string_slice c = extract_number(current);
    skip_white_space(current);
    if(*current != ']') {
        throw std::invalid_argument("invalid cellname");
    }
    ++current;
    skip_white_space(current);
    if(*current) {
        throw std::invalid_argument("invalid cellname");
    }
    m_row = slice_to_i(r) * sign1;
    m_column = slice_to_i(c) * sign2;
}

int relative_cellname::row() const {
    return m_row;
}

int relative_cellname::column() const {
    return m_column;
}

std::string relative_cellname::to_name() const {
    return std::string("R[") +
           std::to_string(m_row) +
           "]C[" + std::to_string(m_column) + "]";
}

relative_cellname::relative_cellname(int r, int c)
    : cellname(r, c) {
}

cellname::cellname(int r, int c)
    : m_row(r), m_column(c) {
}
