#ifndef TABLES_V1_EXPRESSION_LEXER_HPP
#define TABLES_V1_EXPRESSION_LEXER_HPP
#include <string>
#include <vector>

enum token_type {
    Number,
    Function,
    Operator,
    absolute_cellname,
    relative_cellname,
};

struct Token {

};

class expression_lexer {
    std::vector<Token*> tokens;
    static constexpr char* operators[] = { "*", "+", "-", "/", "==", "!=", "<=", ">=" };
    static constexpr char* functions[] = { "if", "and", "not", "or", "sum", "count" };
public:
    expression_lexer() = default;
    void parse(const std::string& text);
};


#endif //TABLES_V1_EXPRESSION_LEXER_HPP
