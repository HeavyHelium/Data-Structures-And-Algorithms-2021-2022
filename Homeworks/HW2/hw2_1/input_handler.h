#ifndef HW2_1_INPUT_HANDLER_H
#define HW2_1_INPUT_HANDLER_H

#include "command_parser.h"
#include "file_parser.h"
#include "string_operations.h"
#include <iostream>
#include "interface.h"
#include <vector>

class input_handler
{
    std::vector<Hierarchy*> branches;
    bool getting_input = false;
public:
    input_handler() = default;
    void getInput()
    {
        getting_input = true;
        std::string line;
        while(getting_input &&
              std::cout << "> " &&
              std::getline(std::cin, line))
        {
            const char* str = line.c_str();
            skip_white_space(str);
            if(is_line_empty(str)) continue;
            command_parser parser;
            try{ execute_command(parser.parse_line(str)); }
            catch(const std::exception& e)
            { std::cout << "error: " << e.what() << "\n"; continue; }
        }

    }
private:
    void execute_command(const command& c)
    {
        switch(c.type)
        {
            case command_type::Help:
            {
                for(const auto& elem: command::commands_supported)
                    std::cout << elem.get_info() << std::endl;
                break;
            }
            case command_type::Load:
            {
                std::string input = data_parser::file_to_string(c.arguments[1]);
                Hierarchy* temp = new Hierarchy(input);
                temp->add_name(c.arguments[0]);
                branches.push_back(temp);
                std::cout << "Successfully loaded branch "
                          << c.arguments[0] << '\n';
                break;
            }
            case command_type::Manager:
            {
                for(Hierarchy* branch : branches)
                {
                    if(branch->get_name() == c.arguments[0])
                    {
                        std::string temp = branch->manager(c.arguments[1]);
                        std::cout << "The manager of "
                                  << c.arguments[1] << " is "
                                  << temp
                                  << ".\n";
                    }
                }
                break;
            }
            case command_type::Exit:
            {
                getting_input = false;
                std::cout << "...exit\n";
                break;
            }
            default: std::cout << "not yet implemented\n"; break;
        }
    }

public:
    ~input_handler()
    {
        for(Hierarchy*& branch : branches)
            delete branch;
    }
};

#endif //HW2_1_INPUT_HANDLER_H
