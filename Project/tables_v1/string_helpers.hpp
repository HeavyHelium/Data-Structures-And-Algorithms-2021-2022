#pragma once
#ifndef TABLES_V1_STRING_HELPERS_HPP
#define TABLES_V1_STRING_HELPERS_HPP

#include <string>
#include <cstring>

enum type {
    Int,
    Double,
    String
};

bool is_white_space(char ch);
bool is_line_empty(const std::string& line);
void skip_white_space(const char*& str);
void skip_spaces_and_tabs(const char*& str);
std::string extract_word(const char*& str);
const char* is_prefix(const char* str, const char* word);
type determine_type(const char* str);


#endif //TABLES_V1_STRING_HELPERS_HPP
