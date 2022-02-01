#pragma once
#ifndef TABLES_V1_INPUT_HANDLER_HPP
#define TABLES_V1_INPUT_HANDLER_HPP
#include <iostream>
#include "command_handler.hpp"
#include "table.hpp"

enum binary_answer {
    yes,
    no,
    error,
};

enum action {
    stop,
    proceed,
    nothing_to_do,
};

class input_handler {
    bool getting_input = false;
    bool saved = true;
    table t;
public:
    void get_input();
    void execute_command(const command& c);
private:
    action reassure_saved() const;
    static binary_answer read_a_binary_answer();
};


#endif //TABLES_V1_INPUT_HANDLER_HPP
