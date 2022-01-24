#ifndef TABLES_V1_EXPRESSION_TOKENIZER_HPP
#define TABLES_V1_EXPRESSION_TOKENIZER_HPP
#include <string>
#include <vector>
#include "Cell.hpp"

class expression_tokenizer {
    std::vector<base_token*> tokens;
    static inline const operator_token operators[] = { operator_token("*", 3, true),
                                                       operator_token("+", 2, true),
                                                       operator_token("-", 2, true),
                                                       operator_token("u-", 4, true),// for unary minus
                                                       operator_token("u+", 4, true),// for unary plus
                                                       operator_token("/", 3, true),
                                                       operator_token("==", 0, true),
                                                       operator_token("!=", 0, true),
                                                       operator_token("<", 1, true),
                                                       operator_token(">", 1, true) };

    static constexpr char* functions[] = { "if", "and", "not", "or", "sum", "count" };
public:
    expression_tokenizer() = default;
    void parse(const std::string& text);
};


#endif //TABLES_V1_EXPRESSION_TOKENIZER_HPP
