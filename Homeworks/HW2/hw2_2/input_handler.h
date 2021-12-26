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
    Hierarchy* find(const std::string& name)
    {
        auto br = branches.find(name);
        if(br == branches.end()) return nullptr;
        return br->second;
    }
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
            case command_type::Find:
            {
                Hierarchy* br = find(c.arguments[0]);
                if(!br) throw std::invalid_argument("no such branch");
                br->find(c.arguments[1]) ?
                                        (std::cout << c.arguments[1] + " is employed in " + c.arguments[0] + "\n"):
                                        (std::cout << c.arguments[1] + " is not employed in " + c.arguments[0] + "\n");
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
                auto br = branches.find(c.arguments[0]);
                if(br == branches.end())
                    throw std::invalid_argument("no such branch");
                std::string temp = br->second->manager(c.arguments[1]);
                std::cout << "The manager of "
                          << c.arguments[1] << " is "
                          << temp
                          << ".\n";
                break;
            }
            case command_type::Exit:
            {
                getting_input = false;
                std::cout << "...exit\n";
                break;
            }
            case command_type::Hire:
            {
                Hierarchy* br = find(c.arguments[0]);
                if(!br) throw std::invalid_argument("no such branch");
                br->hire(c.arguments[1], c.arguments[2]);
                std::cout << "Successfully added " + c.arguments[1] + " in branch\n";
                break;
            }
            case command_type::Fire:
            {
                Hierarchy* br = find(c.arguments[0]);
                if(!br) throw std::invalid_argument("no such branch");
                br->fire(c.arguments[1]);
                std::cout << "Successfully fired " + c.arguments[1] + " from branch\n";
                break;
            }
            case command_type::Num_subordinates:
            {
                Hierarchy* br = find(c.arguments[0]);
                if(!br) throw std::invalid_argument("no such branch");
                std::cout << c.arguments[1] + " has "
                    << br->num_subordinates(c.arguments[1]) << " subordinates\n";
                break;
            }
            case command_type::Num_employees:
            {
                Hierarchy* br = find(c.arguments[0]);
                if(!br) throw std::invalid_argument("no such branch");
                std::cout << c.arguments[0] + " has "
                          << br->num_employees() << " people employed\n";
                break;
            }
            case command_type::Longest_chain:
            {
                Hierarchy* br = find(c.arguments[0]);
                if(!br) throw std::invalid_argument("no such branch");
                std::cout << c.arguments[0] + "'s longest chain has length: "
                          << br->longest_chain() << "\n";
                break;
            }
            case command_type::Salary:
            {
                Hierarchy* br = find(c.arguments[0]);
                if(!br) throw std::invalid_argument("no such branch");
                int salary = br->getSalary(c.arguments[1]);
                std::cout << c.arguments[1] + "'s salary is " << salary << std::endl;
                break;
            }
            case command_type::Overloaded:
            {
                Hierarchy* br = find(c.arguments[0]);
                if(!br) throw std::invalid_argument("no such branch");
                int cnt = br->num_overloaded();
                std::cout << cnt << std::endl;
                std::cout << "in " + c.arguments[0] + " there are "
                          << cnt << " overloaded employees.\n";
                break;
            }
            case command_type::Incorporate:
            {
                Hierarchy* br = find(c.arguments[0]);
                if(!br) throw std::invalid_argument("no such branch");
                br->incorporate();
                std::cout << "Successfully incorporated " + c.arguments[0] + "\n";
                break;
            }
            case command_type::Modernize:
            {
                Hierarchy* br = find(c.arguments[0]);
                if(!br) throw std::invalid_argument("no such branch");
                br->modernize();
                std::cout << "Successfully modernized " + c.arguments[0] + "\n";
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
