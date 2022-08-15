#pragma once
#ifndef TABLES_V1_STRING_HELPERS_HPP
#define TABLES_V1_STRING_HELPERS_HPP

#include <string>
#include <cstring>
#include <cstddef>

enum type {
    Int,
    Double,
    String
};

struct valid_value {
    bool valid = false;
    std::string value;
};

struct string_slice {
    const char* beg = nullptr;
    std::size_t len = 0;
    bool operator==(const string_slice& other) const;
    friend std::ostream& operator<<(std::ostream& os, const string_slice& sl);
};
/// turns part of a cstring to integer
int slice_to_i(const string_slice& sl);
bool is_white_space(char ch);
bool is_line_empty(const std::string& line);
void skip_white_space(const char*& str);
void skip_spaces_and_tabs(const char*& str);
std::string extract_word(const char*& str);
string_slice is_prefix(const char* str, const char* word);
/// determines the type of a string
type determine_type(const char* str);
///@brief extracts a number and from a cstring
///@returns a part of the original string
string_slice extract_number(const char*& str);
///@brief tells whether or not a string stating and ending with " is valid
valid_value valid_string(const char* str);


#endif //TABLES_V1_STRING_HELPERS_HPP
