#pragma once
#ifndef TABLES_V1_EXPRESSION_CELL_HPP
#define TABLES_V1_EXPRESSION_CELL_HPP
#include "Cell.hpp"
#include <unordered_map>
#include <queue>

class base_cell;

class expression_cell : public base_cell {
    static inline const std::unordered_multimap<const char*, operator_token> operators = {
            { "*",  operator_token{ operator_type::Multiply,   3, true } },
            { "+",  operator_token{ operator_type::Plus,       2, true } },
            { "-",  operator_token{ operator_type::Minus,      2, true } },
            { "-",  operator_token{ operator_type::U_minus,    5, false} },
            { "+",  operator_token{ operator_type::U_plus,     5, false} },
            { "/",  operator_token{ operator_type::Div,        3, true } },
            { "==", operator_token{ operator_type::Equals,     0, true } },
            { "!=", operator_token{ operator_type::NotEquals,  0, true } },
            { "<",  operator_token{ operator_type::Smaller,    1, true } },
            { ">",  operator_token{ operator_type::Greater,    1, true } },
            { "(",  operator_token{ operator_type::L_paren,   -1, false} },
            { ")",  operator_token{ operator_type::R_paren,   -1, false} },
            { ",",  operator_token{ operator_type::Separator, -1, false} },
    };
    static inline const std::unordered_multimap<const char*, function_token> functions = {
            { "sum",   function_token{ function_type::Sum,   2 } },
            { "count", function_token{ function_type::Count, 2 } },
            { "if",    function_token{ function_type::If,    3 } },
            { "and",   function_token{ function_type::And,  -1 } },
            { "or",    function_token{ function_type::Or,   -1 } },
            { "not",   function_token{ function_type::Not,  -1 } },
            { "sin",   function_token{ function_type::Sin,   1 } },
            { "cos",   function_token{ function_type::Cos,   1 } },
    };
    numeric_value val;
    absolute_cellname name;
    std::vector<base_token*> tokens;
public:
    /// @brief the table link is only important when initialization occurs
    /// otherwise the expression cell represents some value(int or double) and
    expression_cell(const absolute_cellname& name, table& table_link, const std::string& text);
    ///@brief tells whether or not a given cellname is contained within tokens
    bool contains(const absolute_cellname& name) const;
    ///@brief calculates the expression
    void evaluate(table& table_link);
    ///@brief gets the expression in the cell
    std::string save_value() const override;
    ///@brief gets the stringified numeric value
    std::string print_value() const override;
    std::string print_expr() const override;
    numeric_value get_numeric() const override;
    /// to allow for virtual cloning
    expression_cell* clone() const override;
    ///@brief Rule of 3
    expression_cell(const expression_cell& other);
    expression_cell& operator=(const expression_cell& other);
    ~expression_cell();
    /// static helper methods
    static std::vector<base_token*> copy_tokens(const std::vector<base_token*> tokens);
    static void free_tokens(std::vector<base_token*> tokens);
    static std::vector<base_token*> tokenize(const char* text);
    static std::string tokens_to_string(const std::vector<base_token*>& tokens);
private:
    /// static helper methods
    static base_token* extract_token(const char*& text, bool& prev_token);
    static operator_token* extract_operator(const char*& text, bool is_prev);
    static function_token* extract_function(const char*& text);
    static absolute_cellname_token* extract_abs_cellname(const char*& text);
    static relative_cellname_token* extract_rel_cellname(const char*& text);
    static int_token* extract_int(const char*& text);
    ///@brief turns expression into a modified reverse polish notation
    static void to_RPN(std::queue<base_token*>& output_queue,
                       const std::vector<base_token*>& tokens);
    ///@brief calculates expression and sets the numeric value
    ///@throws when expression is incorrect semantically or its format is invalid
    void calculate(std::queue<base_token*>& output_queue, table& table_link);
    ///@brief calculates functions sum and count and gets rid of them;
    ///this is because they are the only functions,
    /// whose parameter cannot be substituted with their values right away
    std::vector<base_token*> calculate_special_functions(const std::vector<base_token*>& tokens, table& table_link);
    ///@brief gets the absolute address of a relative address cell
    absolute_cellname to_absolute(const relative_cellname& other_name, table& table_link);
};


#endif //TABLES_V1_EXPRESSION_CELL_HPP
