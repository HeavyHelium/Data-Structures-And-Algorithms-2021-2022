#include "expression_cell.hpp"
#include "cellname.hpp"
#include <typeinfo>
#include <stdexcept>
#include <iostream>
#include <cassert>
#include <stack>
#include "table.hpp"

static bool isSeparator(const base_token* p);
static bool isL_paren(const base_token* p);
static bool isR_paren(const base_token* p);
static bool isFunc(const base_token* p);
static bool isOperator(const base_token* p);
static bool isAbsCellname(const base_token* p);
static bool isRelCellname(const base_token* p);
static bool isInt(const base_token* p);

expression_cell::expression_cell(const expression_cell& other)
        : name(other.name),
          val(other.val),
          tokens(copy_tokens(other.tokens))
{}

expression_cell& expression_cell::operator=(const expression_cell &other) {
    if(this != & other) {
        std::vector<base_token *> new_tokens(copy_tokens(other.tokens));
        free_tokens(tokens);
        val = other.val;
        name = other.name;
        tokens = std::move(new_tokens);
    }
    return *this;
}

std::vector<base_token *> expression_cell::copy_tokens(const std::vector<base_token*> tokens) {
    std::vector<base_token*> result;
    for(const base_token* ptr : tokens) {
        result.push_back(ptr->clone());
    }
    return result;
}

void expression_cell::free_tokens(std::vector<base_token *> tokens) {
    for(base_token* ptr : tokens) {
        delete ptr;
    }
}

numeric_value expression_cell::get_numeric() const {
    return val;
}

expression_cell* expression_cell::clone() const {
    return new expression_cell(*this);
}
/// turns tokens into RPN,
/// calculates that RPN
void expression_cell::evaluate(table& table_link) {
    std::queue<base_token*> output_queue;
    std::vector<base_token*> filtered_tokens = calculate_special_functions(tokens, table_link);
    to_RPN(output_queue, filtered_tokens);
    calculate(output_queue, table_link);
    free_tokens(filtered_tokens);

}

std::string expression_cell::print_value() const {
    return val.print_value();
}

expression_cell::expression_cell(const absolute_cellname &name, table& table_link, const std::string& text)
        : name(name) {
    tokens = tokenize(text.c_str());
    evaluate(table_link);
}

expression_cell::~expression_cell() {
    for(base_token* token : tokens) {
        delete token;
    }
}

std::string expression_cell::save_value() const {
    return val.print_value();
}

bool expression_cell::contains(const absolute_cellname& name) const {
    for(base_token* ptr : tokens) {
        if(typeid(*ptr) == typeid(absolute_cellname_token)) {
            const absolute_cellname_token* temp = dynamic_cast<const absolute_cellname_token*>(ptr);
            if(name.row() == temp->name.row() &&
               name.column() == temp->name.column()) {
                    return true;
            }
        } else if(typeid(*ptr) == typeid(relative_cellname)) {
            const relative_cellname_token* temp = dynamic_cast<const relative_cellname_token*>(ptr);
            int actual_r = temp->name.row() + this->name.row();
            int actual_c = temp->name.column() + this->name.column();
            if(actual_r == name.row() &&
               actual_c == name.column()) {
                return true;
            }
        }
    }
    return false;
}

std::vector<base_token*> expression_cell::tokenize(const char* text) {
    bool prev_op = true;
    std::vector<base_token*> result;
    skip_white_space(text);
    while(*text) {
        base_token* e = extract_token(text, prev_op);
        if(!e) {
            free_tokens(result);
            throw std::invalid_argument("unknown token");
        }
        skip_white_space(text);
        result.push_back(e);
    }
    return result;
}
/// tokens are operators, integers, function names and cell addresses(relative or absolute)
base_token *expression_cell::extract_token(const char*& current, bool& prev_op) {
    operator_token* op = extract_operator(current, prev_op);
    if(op) {
        if(op->t != operator_type::R_paren) {
            prev_op = true;
        }
        else prev_op = false;
        skip_white_space(current);
        return op;
    }
    int_token* i = extract_int(current);
    if(i) {
        prev_op = false;
        skip_white_space(current);
        return i;
    }
    function_token* f = extract_function(current);
    if(f) {
        prev_op = false;
        skip_white_space(current);
        return f;
    }
    absolute_cellname_token* a = extract_abs_cellname(current);
    if(a) {
        prev_op = false;
        skip_white_space(current);
        return a;
    }
    relative_cellname_token* r = extract_rel_cellname(current);
    if(r) {
        prev_op = false;
        skip_white_space(current);
        return r;
    }
    return nullptr;
}

operator_token* expression_cell::extract_operator(const char*& text, bool prev_op) {
    for(auto& op_pair : operators) {
        string_slice sl = is_prefix(op_pair.first, text);
        if(sl.len) {
            /// in case there is a unary and binary operator of the same notation
            if(prev_op &&
               op_pair.second.t == operator_type::Plus ||
               prev_op && op_pair.second.t == operator_type::Minus) {
                continue;
            }
            if(!prev_op &&
               op_pair.second.t == operator_type::U_plus ||
               !prev_op && op_pair.second.t == operator_type::U_minus) {
                continue;
            }

            operator_token* new_cell = new operator_token(op_pair.second);
            text += sl.len;
            return new_cell;
        }
    }
    return nullptr;
}

function_token* expression_cell::extract_function(const char*& text) {
    for(auto& f_pair : functions) {
        string_slice sl = is_prefix(f_pair.first, text);
        if(sl.len) {
            function_token* new_cell = new function_token(f_pair.second);
            text += sl.len;
            return new_cell;
        }
    }
    return nullptr;
}

absolute_cellname_token* expression_cell::extract_abs_cellname(const char*& text) {
    const char* temp = text;
    if(*temp != 'R') return nullptr;
    ++temp;
    string_slice r = extract_number(temp);
    if(!r.len) return nullptr;
    if(*temp != 'C') return nullptr;
    ++temp;
    string_slice c = extract_number(temp);
    if(!c.len) {
        return nullptr;
    }
    int num_c = slice_to_i(c);
    int num_r = slice_to_i(r);
    absolute_cellname_token* new_token = new absolute_cellname_token(num_r, num_c);
    text = temp;
    return new_token;
}

relative_cellname_token *expression_cell::extract_rel_cellname(const char*& text) {
    const char* temp = text;
    if(*temp != 'R') return nullptr;
    ++temp;
    skip_white_space(temp);
    if(*temp != '[') return nullptr;
    ++temp;
    skip_white_space(temp);
    int sign1 = 1;
    if((*temp) == '-') {
        ++temp;
        sign1 = -1;
    }
    skip_white_space(temp);
    string_slice r = extract_number(temp);
    if(!r.len) return nullptr;
    skip_white_space(temp);
    if(*temp != ']') {
        return nullptr;
    }
    ++temp;
    skip_white_space(temp);
    if(*temp != 'C') return nullptr;
    ++temp;
    skip_white_space(temp);
    if(*temp != '[') {
        return nullptr;
    }
    ++temp;
    skip_white_space(temp);
    int sign2 = 1;
    if(*temp == '-') {
        sign2 = -1;
        ++temp;
    }
    skip_white_space(temp);
    string_slice c = extract_number(temp);
    if(!c.len) {
        return nullptr;
    }
    skip_white_space(temp);
    if(*temp != ']') {
        return nullptr;
    }
    ++temp;
    int num_c = slice_to_i(c) * sign2;
    int num_r = slice_to_i(r) * sign1;
    relative_cellname_token* new_token = new relative_cellname_token(num_r, num_c);
    text = temp;
    return new_token;
}

int_token* expression_cell::extract_int(const char*& text) {
    const char* temp = text;
    string_slice num = extract_number(temp);
    if(num.len == 0) {
        return nullptr;
    }
    value val{ type::Int };
    val.i_val = slice_to_i(num);
    numeric_value v{ type::Int, val };
    int_token* new_cell = new int_token{ v };
    text = temp;
    return new_cell;
}

std::string expression_cell::tokens_to_string(const std::vector<base_token*>& tokens) {
    std::string res;
    for(const base_token* t : tokens) {
        res += t->save_value();
        res += ' ';
    }
    return res;
}

void expression_cell::to_RPN(std::queue<base_token *>& output_queue,
                             const std::vector<base_token*>& tokens) {
    assert(output_queue.empty());
    std::stack<base_token*> operator_stack;
    /// with 2 more stacks we can handle nested function calls
    /// and functions with variable count of arguments
    std::stack<bool> were_values_stack;
    std::stack<int> arg_cnt_stack;
    for(base_token* token : tokens) {
        /// if it is not an operator or a function name, then
        /// push it onto the output queue
        if(typeid(*token) == typeid(absolute_cellname_token) ||
           typeid(*token) == typeid(relative_cellname_token) ||
           typeid(*token) == typeid(int_token)) {
            output_queue.push(token);
            if(!were_values_stack.empty()) {
                were_values_stack.pop();
                were_values_stack.push(true);
            }
            continue;
        }
        /// if it is a function name:
        if(typeid(*token) == typeid(function_token)) {
            /// push name onto the stack
            operator_stack.push(token);
            /// we came across a new function
            arg_cnt_stack.push(0);
            if(!were_values_stack.empty()) {
                were_values_stack.pop();
                were_values_stack.push(true);
            }
            were_values_stack.push(false);
            continue;
        }
        else if(isSeparator(token)) {
            if(operator_stack.empty()) {
                throw std::invalid_argument("wrong expression format");
            }
            while(!operator_stack.empty()) {
                base_token* top = operator_stack.top();
                if(isL_paren(top)) break;
                output_queue.push(top);
                operator_stack.pop();
            }
            if(operator_stack.empty()) {
                throw std::invalid_argument("wrong expression format");
            }
            assert(!were_values_stack.empty());
            bool w = were_values_stack.top();
            if(w) {
                assert(!arg_cnt_stack.empty());
                ++arg_cnt_stack.top();
            }
            were_values_stack.push(false);
        } else if(isL_paren(token)) {
            operator_stack.push(token);
        } else if(isR_paren(token)) {
            while(!operator_stack.empty() &&
                  !isL_paren(operator_stack.top())) {
                output_queue.push(operator_stack.top());
                operator_stack.pop();
            }
            if(operator_stack.empty()) {
                throw std::invalid_argument("wrong expression format");
             }
            operator_stack.pop();
            if(!operator_stack.empty() && isFunc(operator_stack.top())) {
                function_token* func = dynamic_cast<function_token*>(operator_stack.top());
                int arg_count = arg_cnt_stack.top();
                arg_cnt_stack.pop();
                bool w = were_values_stack.top();
                were_values_stack.pop();
                if(w) ++ arg_count;
                func->argument_count = arg_count;
                operator_stack.pop();
                output_queue.push(func);
            }
        } else {
            /// it is a typical operator
            assert(isOperator(token));
            operator_token* current = dynamic_cast<operator_token*>(token);
            while(!operator_stack.empty() &&
                  isOperator(operator_stack.top())) {
                operator_token* top = dynamic_cast<operator_token*>(operator_stack.top());
                if(current->left_associative &&
                   current->precedence <= top->precedence ||
                    (!current->left_associative &&
                      current->precedence < top->precedence)) {
                    operator_stack.pop();
                    output_queue.push(top);
                }
                else {
                    break;
                }
            }
            operator_stack.push(token);
        }
    }
    while(!operator_stack.empty()) {
        output_queue.push(operator_stack.top());
        operator_stack.pop();
    }

}

void expression_cell::calculate(std::queue<base_token*> &output_queue, table& table_link) {
    std::stack<numeric_value> calculation_stack;
    while(!output_queue.empty()) {
        base_token* token = output_queue.front();
        output_queue.pop();
        if(isInt(token)) {
            int_token* temp = dynamic_cast<int_token*>(token);
            calculation_stack.push(temp->val);
        } else if(isAbsCellname(token)) {
            absolute_cellname_token* temp = dynamic_cast<absolute_cellname_token*>(token);
            if(temp->name == name) {
                throw std::invalid_argument("cyclic dependencies are not allowed");
            }
            table_link.validate_address(temp->name);
            auto found_cell = table_link.find_cell(temp->name);
            if(found_cell != table_link.t.end()) {
                if(typeid(*found_cell->second) == typeid(expression_cell)) {
                    expression_cell* temp = dynamic_cast<expression_cell*>(found_cell->second);
                    if(temp->contains(name)) {
                        throw std::invalid_argument("cyclic dependencies are not allowed");
                    }
                }
            }
            calculation_stack.push(table_link.get_num_value(temp->name));
        } else if(isRelCellname(token)) {
            relative_cellname_token* temp = dynamic_cast<relative_cellname_token*>(token);
            absolute_cellname actual(to_absolute(temp->name, table_link));
            if(actual == name) {
                throw std::invalid_argument("cyclic dependencies are not allowed");
            }
            auto found_cell = table_link.find_cell(actual);
            if(found_cell != table_link.t.end()) {
                if(typeid(*found_cell->second) == typeid(expression_cell)) {
                    expression_cell* temp = dynamic_cast<expression_cell*>(found_cell->second);
                    if(temp->contains(name)) {
                        throw std::invalid_argument("cyclic dependencies are not allowed");
                    }
                }
            }
            calculation_stack.push(table_link.get_num_value(actual));
        } else if(isOperator(token)) {
            operator_token* temp = dynamic_cast<operator_token*>(token);
            switch(temp->t) {
                case operator_type::U_minus : {
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value temp = calculation_stack.top();
                    temp = -temp;
                    calculation_stack.pop();
                    calculation_stack.push(temp);
                    break;
                }
                case operator_type::U_plus : {
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    break;
                }
                case operator_type::Plus : {
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand2 = calculation_stack.top();
                    calculation_stack.pop();
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand1 = calculation_stack.top();
                    calculation_stack.pop();
                    operand1 += operand2;
                    calculation_stack.push(operand1);
                    break;
                }
                case operator_type::Minus : {
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand2 = calculation_stack.top();
                    calculation_stack.pop();
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand1 = calculation_stack.top();
                    calculation_stack.pop();
                    operand1 -= operand2;
                    calculation_stack.push(operand1);
                    break;
                }
                case operator_type::Multiply : {
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand2 = calculation_stack.top();
                    calculation_stack.pop();
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand1 = calculation_stack.top();
                    calculation_stack.pop();
                    operand1 *= operand2;
                    calculation_stack.push(operand1);
                    break;
                }
                case operator_type::Div : {
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand2 = calculation_stack.top();
                    calculation_stack.pop();
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand1 = calculation_stack.top();
                    calculation_stack.pop();
                    if(!operand2) {
                        throw std::logic_error("division by zero is not allowed");
                    }
                    operand1 /= operand2;
                    calculation_stack.push(operand1);
                    break;
                }
                case operator_type::Equals : {
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand2 = calculation_stack.top();
                    calculation_stack.pop();
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand1 = calculation_stack.top();
                    calculation_stack.pop();
                    numeric_value res = operand1 == operand2;
                    calculation_stack.push(res);
                    break;
                }
                case operator_type::NotEquals : {
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand2 = calculation_stack.top();
                    calculation_stack.pop();
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand1 = calculation_stack.top();
                    calculation_stack.pop();
                    numeric_value res = operand1 != operand2;
                    calculation_stack.push(res);
                    break;
                }
                case operator_type::Smaller : {
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand2 = calculation_stack.top();
                    calculation_stack.pop();
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand1 = calculation_stack.top();
                    calculation_stack.pop();
                    numeric_value res = operand1 < operand2;
                    calculation_stack.push(res);
                    break;
                }
                case operator_type::Greater : {
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand2 = calculation_stack.top();
                    calculation_stack.pop();
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value operand1 = calculation_stack.top();
                    calculation_stack.pop();
                    numeric_value res = operand1 > operand2;
                    calculation_stack.push(res);
                    break;
                }
                default: {
                    throw std::invalid_argument("invalid expression format");
                    break;
                }
            }
        } else if(isFunc(token)) {
            function_token* f = dynamic_cast<function_token*>(token);
            switch(f->t) {
                case function_type::If : {
                    if(f->argument_count != 3) {
                        throw std::invalid_argument("invalid argument count");
                    }
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value arg3 = calculation_stack.top();
                    calculation_stack.pop();
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value arg2 = calculation_stack.top();
                    calculation_stack.pop();
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value arg1 = calculation_stack.top();
                    calculation_stack.pop();
                    numeric_value res = arg1 ? arg2 : arg3;
                    calculation_stack.push(res);
                    break;
                }
                case function_type::And : {
                    int arg_cnt = f->argument_count;
                    if(!arg_cnt) {
                        throw std::invalid_argument("wrong argument count");
                    }
                    bool res = true;
                    while(arg_cnt--) {
                        if(calculation_stack.empty()) {
                            throw std::invalid_argument("wrong expression format");
                        }
                        res = res && calculation_stack.top();
                        calculation_stack.pop();
                    }
                    numeric_value value_res{ type::Int };
                    value_res.V.i_val = res;
                    calculation_stack.push(value_res);
                    break;
                }
                case function_type::Or : {
                    int arg_cnt = f->argument_count;
                    if(!f) {
                        throw std::invalid_argument("wrong argument count");
                    }
                    bool res = false;
                    while(arg_cnt--) {
                        if(calculation_stack.empty()) {
                            throw std::invalid_argument("wrong expression format");
                        }
                        res = res || calculation_stack.top();
                        calculation_stack.pop();
                    }
                    numeric_value value_res{ type::Int };
                    value_res.V.i_val = res;
                    calculation_stack.push(value_res);
                    break;
                }
                case function_type::Not : {
                    if(f->argument_count != 1 || calculation_stack.empty())
                        throw std::invalid_argument("wrong expression format");
                    numeric_value top = calculation_stack.top();
                    calculation_stack.pop();
                    calculation_stack.push(!top);
                    break;
                }
                case function_type::Sin : {
                    if(f->argument_count != 1) {
                        throw std::invalid_argument("invalid argument count");
                    }
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value arg1 = calculation_stack.top();
                    calculation_stack.pop();
                    numeric_value res = numeric_value::sin(arg1);
                    calculation_stack.push(res);
                    break;
                }
                case function_type::Cos : {
                    if(f->argument_count != 1) {
                        throw std::invalid_argument("invalid argument count");
                    }
                    if(calculation_stack.empty()) {
                        throw std::invalid_argument("expression is incorrect");
                    }
                    numeric_value arg1 = calculation_stack.top();
                    calculation_stack.pop();
                    numeric_value res = numeric_value::cos(arg1);
                    calculation_stack.push(res);
                    break;
                }
                default: {
                    std::cerr << "error\n";
                }
            }
        }
    }
    if(calculation_stack.empty()) {
        throw std::invalid_argument("calculation error");
    }
    numeric_value res = calculation_stack.top();
    calculation_stack.pop();
    if(!calculation_stack.empty()) {
        throw std::invalid_argument("calculation error");
    }
    val = res;
}

std::vector<base_token *> expression_cell::calculate_special_functions(const std::vector<base_token *> &tokens,
                                                                       table& table_link) {
    std::vector<base_token*> result;
    for(std::size_t i = 0; i < tokens.size(); ++i) {
        if(isFunc(tokens[i])) {
            const function_token* f = dynamic_cast<const function_token*>(tokens[i]);
            bool sum = f->t == function_type::Sum;
            bool count = f->t == function_type::Count;
            if(sum || count) {
                if(i + 1 == tokens.size() || !isL_paren(tokens[i + 1])) {
                     throw std::invalid_argument("wrong expression format");
                }
                ++i; // we are now at the left parenthesis
                if(i + 1 == tokens.size() ||
                   (!isRelCellname(tokens[i + 1]) &&
                   !isAbsCellname(tokens[i + 1]))) {
                    throw std::invalid_argument("wrong expression format");
                }
                ++i; // we are now at the first argument
                // next must be a separator
                if(i + 1 == tokens.size() ||
                   !isSeparator(tokens[i + 1])) {
                    throw std::invalid_argument("wrong expression format");
                }
                ++i; // we are not at the separator
                // next must be an address
                if(i + 1 == tokens.size() ||
                   (!isRelCellname(tokens[i + 1]) &&
                   !isAbsCellname(tokens[i + 1]))) {
                    throw std::invalid_argument("wrong expression format");
                }
                ++i; // we are now at the second address
                // next must be a right parenthesis
                if(i + 1 == tokens.size() ||
                   !isR_paren(tokens[i + 1])) {
                    throw std::invalid_argument("wrong expression format");
                }
                ++i; // we are now at the parenthesis
                absolute_cellname op1(0, 0);
                absolute_cellname op2(0, 0);
                if(isAbsCellname(tokens[i - 3])) {
                    absolute_cellname_token* t = dynamic_cast<absolute_cellname_token*>(tokens[i - 3]);
                    op1 = t->name;
                    table_link.validate_address(op1);
                } else {
                    relative_cellname_token* t = dynamic_cast<relative_cellname_token*>(tokens[i - 3]);
                    op1 = to_absolute(t->name, table_link);
                }
                if(isAbsCellname(tokens[i - 1])) {
                    absolute_cellname_token* t = dynamic_cast<absolute_cellname_token*>(tokens[i - 1]);
                    op2 = t->name;
                    table_link.validate_address(op2);
                } else {
                    relative_cellname_token* t = dynamic_cast<relative_cellname_token*>(tokens[i - 1]);
                    op2 = to_absolute(t->name, table_link);
                }
                numeric_value res;
                if(op1.row() <= name.row() && op2.row() >= name.row() &&
                   op1.column() <= name.column() && op2.column() >= name.column()) {
                    throw std::invalid_argument("cyclic dependencies are not allowed");
                }
                if(sum) res = table_link.sum_area(op1, op2);
                else {
                    res.T = type::Int;
                    int cnt = table_link.count_area(op1, op2);
                    res.V.i_val = cnt;
                }
                int_token* new_token = new int_token(res);
                result.push_back(new_token);
            }
            else {
                result.push_back(tokens[i]->clone());
            }
        }
        else {
            result.push_back(tokens[i]->clone());
        }
    }
    return result;
}

bool isL_paren(const base_token* p) {
    if(!p) return false;
    if(typeid(*p) == typeid(operator_token)) {
        const operator_token* temp = dynamic_cast<const operator_token*>(p);
        if(temp->t == operator_type::L_paren) {
            return true;
        }
    }
    return false;
}

bool isR_paren(const base_token* p) {
    if(!p) return false;
    if(typeid(*p) == typeid(operator_token)) {
        const operator_token* temp = dynamic_cast<const operator_token*>(p);
        if(temp->t == operator_type::R_paren) {
            return true;
        }
    }
    return false;
}

bool isSeparator(const base_token* p) {
    if(!p) return false;
    if(typeid(*p) == typeid(operator_token)) {
        const operator_token* temp = dynamic_cast<const operator_token*>(p);
        if(temp->t == operator_type::Separator) {
            return true;
        }
    }
    return false;
}

bool isFunc(const base_token* p) {
    if(!p) return false;
    if(typeid(*p) == typeid(function_token)) {
        return true;
    }
    return false;
}

bool isInt(const base_token* p) {
    if(!p) return false;
    if(typeid(*p) == typeid(int_token)) {
        return true;
    }
    return false;
}

bool isOperator(const base_token* p) {
    if(!p) return false;
    if(typeid(*p) == typeid(operator_token)) {
        return true;
    }
    return false;
}

bool isAbsCellname(const base_token* p) {
    if(!p) return false;
    if(typeid(*p) == typeid(absolute_cellname_token)) {
        return true;
    }
    return false;
}

bool isRelCellname(const base_token* p) {
    if(!p) return false;
    if(typeid(*p) == typeid(relative_cellname_token)) {
        return true;
    }
    return false;
}

absolute_cellname expression_cell::to_absolute(const relative_cellname& other_name,
                                               table& table_link) {
    int actual_r = name.row() + other_name.row();
    int actual_c = name.column() + other_name.column();
    absolute_cellname actual(actual_r, actual_c);
    table_link.validate_address(actual);
    return actual;
}

std::string expression_cell::print_expr() const {
    std::string res;
    bool interval = false;
    for(const base_token* ptr : tokens) {
        res += ptr->save_value();
        res += ' ';
    }
    return res;
}
