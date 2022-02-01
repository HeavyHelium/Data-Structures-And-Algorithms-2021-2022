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
            std::cout << "hakuna_matata print_val_all\n";
            break;
        }
        case print_expr_all: {
            std::cout << "hakuna_matata print_expr_all\n";
            break;
        }
        case save: {
            t.save(c.arguments[0]);
            break;
        }
        case load: {
            t.load(c.arguments[0]);
            break;
        }
        case incr: {
            t.incr(c.arguments[0]);
            break;
        }
        case dcr: {
            t.dcr(c.arguments[0]);
            break;
        }
        case command_type::Exit: {
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
