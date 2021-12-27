#ifndef HW2_1_INPUT_HANDLER_H
#define HW2_1_INPUT_HANDLER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include "command_parser.h"
#include "data_parser.h"
#include "string_operations.h"
#include "interface.h"

/// deals with command-line interface
class input_handler
{
    std::unordered_map<std::string, Hierarchy*> branches;
    bool getting_input = false;
public:
    input_handler() = default;
    void getInput();
private:
    Hierarchy* find(const std::string& name);
    void execute_command(const command& c);
public:
    ~input_handler();
};

#endif //HW2_1_INPUT_HANDLER_H
