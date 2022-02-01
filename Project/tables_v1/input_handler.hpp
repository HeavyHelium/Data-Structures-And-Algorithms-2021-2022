#pragma once
#ifndef TABLES_V1_INPUT_HANDLER_HPP
#define TABLES_V1_INPUT_HANDLER_HPP
#include <iostream>
#include "command_handler.hpp"
#include "table.hpp"

class input_handler {
    bool getting_input = false;
    table t;
public:
    void get_input();
    void execute_command(const command& c);
};


#endif //TABLES_V1_INPUT_HANDLER_HPP
