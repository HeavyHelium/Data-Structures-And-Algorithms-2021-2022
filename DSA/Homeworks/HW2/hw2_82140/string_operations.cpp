#include "string_operations.h"
#include <string>
#include <cstring>

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


