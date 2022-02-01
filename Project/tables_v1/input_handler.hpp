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
/// deals with command-line interface
class input_handler {
    bool getting_input = false;
    bool saved = true;
    table t;
public:
    void get_input();
    ///@brief executed a command extracted from input
    void execute_command(const command& c);
private:
    ///@brief checks whether current file is not saved and asks user if they wish to proceed
    action reassure_saved() const;
    ///@brief reads a y/n answer
    static binary_answer read_a_binary_answer();
};


#endif //TABLES_V1_INPUT_HANDLER_HPP
