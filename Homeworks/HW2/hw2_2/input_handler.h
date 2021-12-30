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
public:
    struct hierarchy_state
    {
        std::string name;
        Hierarchy* hierarchy = nullptr;
        //std::string file_path;
        bool saved = false;
        hierarchy_state(Hierarchy* h, const std::string& name, bool saved = false);
    };
    input_handler() = default;
    ~input_handler();
    void getInput();
private:
    typename std::vector<input_handler::hierarchy_state>::iterator find_message(const std::string& name);
    typename std::vector<input_handler::hierarchy_state>::iterator find(const std::string& name);
    void execute_command(const command& c);
    std::vector<hierarchy_state> hierarchies;
    bool getting_input = false;
};

#endif //HW2_1_INPUT_HANDLER_H
