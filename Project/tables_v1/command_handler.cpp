#include "command_handler.hpp"
#include "string_helpers.hpp"
#include <stdexcept>
#include <iostream>

command command_parser::parse(const char* line) {
    command_type type = parse_name(line);
    switch (type) {
        case set: {
            skip_white_space(line);
            if(!(*line)) {
                throw std::invalid_argument("wrong argument count\n");
            }
            string_slice address = get_next_word(line);
            if(!(*line)) {
                throw std::invalid_argument("wrong argument count\n");
            }
            skip_white_space(line);
            if(!*line) {
                throw std::invalid_argument("wrong argument count\n");
            }
            return command{ type, { std::string(address.beg, address.len), line } };

        }
        case print_val: {
            skip_white_space(line);
            if(!(*line)) {
                throw std::invalid_argument("wrong argument count\n");
            }
            string_slice address = get_next_word(line);
            skip_white_space(line);
            if(*line) {
                throw std::invalid_argument("wrong argument count\n");
            }
            return command{ type, { std::string(address.beg, address.len) } };
        }
        case print_expr: {
            skip_white_space(line);
            if(!(*line)) {
                throw std::invalid_argument("wrong argument count\n");
            }
            string_slice address = get_next_word(line);
            skip_white_space(line);
            if(*line) {
                throw std::invalid_argument("wrong argument count\n");
            }
            return command{ type, { std::string(address.beg, address.len) } };
        }
        case print_val_all: {
            skip_white_space(line);
            if(*line) {
                throw std::invalid_argument("wrong argument count\n");
            }
            return command{ type };
        }
        case print_expr_all: {
            skip_white_space(line);
            if(line) {
                throw std::invalid_argument("wrong argument count\n");
            }
            return command{ type };
        }
        case save: {
            skip_white_space(line);
            if(!(*line)) {
                throw std::invalid_argument("wrong argument count\n");
            }
            string_slice filename = get_next_word(line);
            skip_white_space(line);
            if(*line) {
                throw std::invalid_argument("wrong argument count\n");
            }
            return command{ type, { std::string(filename.beg, filename.len) } };
        }
        case load: {
            skip_white_space(line);
            if(!(*line)) {
                throw std::invalid_argument("wrong argument count\n");
            }
            string_slice filename = get_next_word(line);
            skip_white_space(line);
            if(*line) {
                throw std::invalid_argument("wrong argument count\n");
            }
            return command{ type, { std::string(filename.beg, filename.len) } };
        }
        case incr: {
            skip_white_space(line);
            if(!(*line)) {
                throw std::invalid_argument("wrong argument count\n");
            }
            string_slice address = get_next_word(line);
            skip_white_space(line);
            if(*line) {
                throw std::invalid_argument("wrong argument count\n");
            }
            return command{ type, { std::string(address.beg, address.len) } };
        }
        case dcr: {
            skip_white_space(line);
            if(!(*line)) {
                throw std::invalid_argument("wrong argument count\n");
            }
            string_slice address = get_next_word(line);
            skip_white_space(line);
            if(*line) {
                throw std::invalid_argument("wrong argument count\n");
            }
            return command{ type, { std::string(address.beg, address.len) } };
        }
        default: {
            std::cout << "not yet implemented/added\n";
        }
        case command_type::Exit: {
            skip_white_space(line);
            if(*line) {
                throw std::invalid_argument("wrong argument count\n");
            }
            return command{ type };
        }
    }
}

command_type command_parser::parse_name(const char*& line) {
    skip_white_space(line);
    if(!*line) {
        throw std::invalid_argument("line is empty");
    }
    for(const_command_info& c : command_parser::supported_commands) {
        string_slice sl = is_prefix(c.first.c_str(), line);
        if(sl.len) {
            line += sl.len;
            return c.second.t;
        }
    }
    throw std::invalid_argument("unrecognized command\n");
}

string_slice command_parser::get_next_word(const char*& line) {
    skip_white_space(line);
    const char* beg = line;
    while(*line && !is_white_space(*line)){
        ++line;
    }
    return string_slice{ beg, static_cast<size_t>(line - beg) };
}

