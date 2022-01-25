#include "expression_tokenizer.hpp"
#include "Cell.hpp"

std::vector<std::string> expression_tokenizer::split(const std::string& text) {
    const char* current = text.c_str();
    std::string token;
    while(*current) {
       // string_slice op_found = get_operator(current);
    }

    return std::vector<std::string>();
}
