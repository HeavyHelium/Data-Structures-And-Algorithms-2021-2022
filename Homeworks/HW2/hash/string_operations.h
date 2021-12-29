#pragma once
#ifndef HW2_1_STRING_OPERATIONS_H
#define HW2_1_STRING_OPERATIONS_H
#include <string>
#include <cstring>

bool is_white_space(char ch);
bool is_line_empty(const std::string& line);
void skip_white_space(const char*& str);
void skip_spaces_and_tabs(const char*& str);
std::string extract_word(const char*& str);



#endif //HW2_1_STRING_OPERATIONS_H
