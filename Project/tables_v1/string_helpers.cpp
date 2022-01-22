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
    return ch == '0' || ch == '1' ||
           ch == '2' || ch == '3' ||
           ch == '4' || ch == '5' ||
           ch == '6' || ch == '7' ||
           ch == '8' || ch == '9';
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