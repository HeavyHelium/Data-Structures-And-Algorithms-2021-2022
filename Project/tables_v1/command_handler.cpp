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
            return command{ type, { std::string(address.begin, address.length), line } };

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
            return command{ type, { std::string(address.begin, address.length) } };
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
            return command{ type, { std::string(address.begin, address.length) } };
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
                std::cout << "here\n";
                throw std::invalid_argument("wrong argument count\n");
            }
            string_slice filename = get_next_word(line);
            skip_white_space(line);
            if(*line) {
                std::cout << "e: " << line << std::endl;
                throw std::invalid_argument("wrong argument count\n");
            }
            return command{ type, { std::string(filename.begin, filename.length) } };
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
            return command{ type, { std::string(filename.begin, filename.length) } };
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
            return command{ type, { std::string(address.begin, address.length) } };
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
            return command{ type, { std::string(address.begin, address.length) } };
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
    const char* begin = line;
    bool found_command = false;
    for(auto& c : command_parser::supported_commands) {
        const char* pos = is_prefix(line, c.first.c_str());
        if(pos != line) {
            found_command = true;
            line = pos;
            return c.second.t;
        }
    }
    if(!found_command) {
        throw std::invalid_argument("unrecognized command");
    }
}

string_slice command_parser::get_next_word(const char*& line) {
    skip_white_space(line);
    const char* beg = line;
    while(*line && !is_white_space(*line)){
        ++line;
    }
    return string_slice{ beg, static_cast<size_t>(line - beg) };
}

