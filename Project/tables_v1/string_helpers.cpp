#include "string_helpers.hpp"
#include <string>
#include <cstring>
#include <iostream>

bool is_white_space(char ch)
{
    return ch == ' ' ||
           ch == '\n'||
           ch == '\t';
}
bool is_line_empty(const std::string& line)
{
    const char* str = line.c_str();
    while(*str)
        if(!is_white_space(*str++)) return false;
    return true;
}
void skip_white_space(const char*& str)
{
    while(*str && is_white_space(*str))
        ++str;
}
void skip_spaces_and_tabs(const char*& str)
{
    while(*str && is_white_space(*str) && *str != '\n')
        ++str;
}
std::string extract_word(const char*& str)
{
    std::string word;
    while(*str && !is_white_space(*str))
        word.push_back(*str++);
    skip_white_space(str);
    return word;
}

const char* is_prefix(const char* str, const char* word) {
    const char* temp = str;
    while(*word) {
        if(*str++ != *word++) {
            return temp;
        }
    }
    return str;
}

bool is_digit(const char ch) {
    return ch >= '0' && ch <= '9';
}

type determine_type(const char* str) {
    while(*str) {
        while(*str && is_digit(*str)) {
            ++str;
        }
        if(!*str) {
            return type::Int;
        }
        if(*str == '.') {
            ++str;
            if(!str) {
                return type::String;
            }
            while(*str && is_digit(*str)) {
                ++str;
            }
            if(!*str) {
                return type::Double;
            }
            return type::String;
        }
    }
    return type::String;
}
// extract a non-negative number
string_slice extract_number(const char*& str) {
    const char* current =  str;
    skip_white_space(current);
    const char* beg = current;
    std::size_t len = 0;
    while(is_digit(*current)) {
        ++current;
        ++len;
    }
    str = current;
    return { beg, len };
}

bool string_slice::operator==(const string_slice &other) const {
    if(len != other.len)
        return false;
    int iter = len;
    const char* l_iter = beg;
    const char* r_iter = other.beg;
    while(iter--) {
        if(*(l_iter++) != *(r_iter++)) {
            return false;
        }
    }
    return true;
}
/// undefined behaviour in case of sl not representing a non-negative number
int slice_to_i(const string_slice& sl) {
    std::size_t len = sl.len;
    const char* iter = sl.beg;
    int number = 0;
    while(len--) {
       number *= 10;
       number += *iter++ - '0';
    }
    return number;
}

valid_value valid_string(const char* str) {
    std::string value;
    if(*str != '\"') return { false };
    const char* current = str;
    ++current;
    while(*current) {
        while(*current && *current != '\"' && *current != '\\') {
            value.push_back(*current);
            ++current;
        }
        if(*current == '\"' && !*(current + 1)) {
            return { true, value };
        }
        if(*current == '\"') {
            return { false };
        }
        if(*current == '\\' && *(current + 1) == '\"' ||
           *current == '\\' && *(current + 1) == '\\') {
            value.push_back(*(current + 1));
            ++++current;
            continue;
        }
        return { false };
    }
    return { false };
}
