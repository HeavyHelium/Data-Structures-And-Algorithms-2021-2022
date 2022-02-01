#pragma once
#ifndef TABLES_V1_COMMAND_HANDLER_HPP
#define TABLES_V1_COMMAND_HANDLER_HPP
#include <string>
#include <vector>
#include <unordered_map>
#include "string_helpers.hpp"

enum command_type {
    set,
    print_val,
    print_expr,
    print_val_all,
    print_expr_all,
    save,
    load,
    incr,
    dcr,
    Exit,
};

struct command {
    command_type type;
    std::vector<std::string> arguments;
};

struct type_args {
    command_type t;
    std::vector<std::string> args;
};

using command_info = std::pair<std::string, type_args>;
using const_command_info = const std::pair<std::string, type_args>;

struct command_parser {
    using arguments_info = std::vector<std::string>;
    std::string name;
    std::vector<std::string> arguments;
    static inline const std::unordered_map<std::string, type_args> supported_commands =     {
        { "SET",            { command_type::set,           { "address", "expression" } } },
        { "PRINT VAL",      { command_type::print_val,     { "address"  }              } },
        { "PRINT EXPR",     { command_type::print_expr,    { "address"  }              } },
        { "PRINT VAL ALL",  { command_type::print_val_all                              } },
        { "PRINT EXPR ALL", { command_type::print_expr_all                             } },
        { "SAVE",           { command_type::save,          { "filename" }              } },
        { "LOAD",           { command_type::load,          { "filename" }              } },
        { "++",             { command_type::incr,          { "address"  }              } },
        { "--",             { command_type::dcr,           { "address"  }              } },
        { "EXIT",           { command_type::Exit                                       } },
    };
    command parse(const char* line);
private:
    command_type parse_name(const char*& line);
    string_slice get_next_word(const char*& line);
};



#endif //TABLES_V1_COMMAND_HANDLER_HPP
