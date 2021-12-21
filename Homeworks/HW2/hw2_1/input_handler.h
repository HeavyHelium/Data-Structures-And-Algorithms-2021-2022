#ifndef HW2_1_INPUT_HANDLER_H
#define HW2_1_INPUT_HANDLER_H

#include "command_parser.h"
#include "data_parser.h"
#include "string_operations.h"
#include <iostream>
#include "interface.h"
#include <vector>
#include <unordered_map>
#include <string>

/// deals with command-line interface
class input_handler
{
    std::unordered_map<std::string, Hierarchy*> branches;
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
                std::string input = (data_parser::file_data(c.arguments[1]));
                Hierarchy* temp = new Hierarchy(input);
                branches.insert({ c.arguments[0], temp });
                std::cout << "Successfully loaded branch "
                          << c.arguments[0] << '\n';
                break;
            }
            case command_type::Load_1arg:
            {
                std::string input = (data_parser::console_data());
                Hierarchy* temp = new Hierarchy(input);
                branches.insert({ c.arguments[0], temp });
                std::cout << "Successfully loaded branch "
                          << c.arguments[0] << '\n';
                break;
            }
            case command_type::Save:
            {
                std::ofstream oFile(c.arguments[1]);
                auto br = branches.find(c.arguments[0]);
                if(br == branches.end()) throw std::invalid_argument("no such branch");
                oFile << br->second->print();
                std::cout << "Successfully saved branch "
                          << c.arguments[0] << '\n';
                break;
            }
            case command_type::Save_1arg:
            {
                auto br = branches.find(c.arguments[0]);
                if(br == branches.end()) throw std::invalid_argument("no such branch");
                std::cout << br->second->print();
                break;
            }
            case command_type::Manager:
            {
                for(const auto& b : branches)
                {
                    std::string temp = b.second->manager(c.arguments[1]);
                    std::cout << "The manager of "
                              << c.arguments[1] << " is "
                              << temp
                              << ".\n";
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
        for(auto& b: branches)
            delete b.second;
    }
};

#endif //HW2_1_INPUT_HANDLER_H
