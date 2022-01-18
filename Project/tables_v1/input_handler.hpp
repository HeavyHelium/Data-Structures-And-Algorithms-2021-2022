#ifndef TABLES_V1_INPUT_HANDLER_HPP
#define TABLES_V1_INPUT_HANDLER_HPP
#include <iostream>
#include "command_handler.hpp"

class input_handler {
    bool getting_input = false;
public:
    void get_input();
    void execute_command(const command& c);
};


#endif //TABLES_V1_INPUT_HANDLER_HPP
