#include "input_handler.hpp"
#include <string>
#include "string_helpers.hpp"
#include "command_handler.hpp"

void input_handler::get_input() {
    getting_input = true;
    std::string line;
    while(getting_input &&
          std::cout << "> " &&
          std::getline(std::cin, line)) {
        if(is_line_empty(line)) {
            continue;
        }
        command_parser h;
        try {
            command c(h.parse(line.c_str()));
            execute_command(c);
        }
        catch(const std::exception& e) {
            std::cerr << "error: " << e.what() << "\n";
        }
    }
}

void input_handler::execute_command(const command &c) {
    switch (c.type) {
        case set: {
            t.set(c.arguments[0], c.arguments[1]);
            saved = false;
            break;
        }
        case print_val: {
            t.print_val(c.arguments[0]);
            break;
        }
        case print_expr: {
            t.print_expr(c.arguments[0]);
            break;
        }
        case print_val_all: {
            t.print_val_all();
            break;
        }
        case print_expr_all: {
            t.print_expr_all();
            break;
        }
        case save: {
            t.save(c.arguments[0]);
            saved = true;
            break;
        }
        case load: {
            t.load(c.arguments[0]);
            if(reassure_saved() == action::stop) {
                return;
            }
            saved = true;
            break;
        }
        case incr: {
            t.incr(c.arguments[0]);
            saved = false;
            break;
        }
        case dcr: {
            t.dcr(c.arguments[0]);
            saved = false;
            break;
        }
        case command_type::Exit: {
            if(reassure_saved() == action::stop) {
                return;
            }
            std::cout << "...Goodbye\n...press any key to close the program\n";
            getting_input = false;
            char ch;
            std::cin.get(ch);
            break;
        }
        default: {
            std::cout << "not yet implemented/added\n";
            break;
        }
    }
}

action input_handler::reassure_saved() const {
    if(!saved) {
        std::cout << "The current document has been modified, but not saved.\n"
                     "Are you sure you want to proceed anyway(y/n)? : ";
        for(;;) {
            binary_answer ans = read_a_binary_answer();
            switch (ans) {
                case binary_answer::no : {
                    return action::stop;
                }
                case binary_answer::yes : {
                    return action::proceed;
                }
                case binary_answer::error : {
                    std::cerr << "please answer with y or n: ";
                    break;
                }
            }
        }
    }
    return action::nothing_to_do;
}

binary_answer input_handler::read_a_binary_answer() {
    std::string answer;
    std::getline(std::cin, answer);
    const char* line = answer.c_str();
    skip_white_space(line);
    if(!*line || (*line != 'y' && *line != 'n')) {
        return binary_answer::error;
    }
    char ch = *line++;
    skip_white_space(line);
    if(*line) {
        return binary_answer::error;
    }
    return ch == 'y' ? binary_answer::yes : binary_answer::no;
}
