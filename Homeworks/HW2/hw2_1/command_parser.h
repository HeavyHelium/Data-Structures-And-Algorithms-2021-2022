#pragma once
#ifndef HW2_1_COMMAND_PARSER_H
#define HW2_1_COMMAND_PARSER_H
#include <vector>
#include <string>
#include "string_operations.h"
#include <stdexcept>

// <name, info>
enum command_type
{
    Help,
    Load,
    Load_1arg,
    Save,
    Save_1arg,
    Find,
    Num_subordinates,
    Manager,
    Num_employees,
    Overloaded,
    Join,
    Fire,
    Hire,
    Salary,
    Incorporate,
    Modernize,
    Exit
};

struct name_info_pair
{
    command_type type;
    std::string name;
    std::vector<std::string> arguments;
    std::string basic_info;
    std::string get_info() const
    {
        std::string info;
        info += name;
        for(const std::string& arg: arguments)
            info += " <" + arg + "> ";
        return "\t" + info + "\n" + "\t\t" + basic_info;
    }
};

struct command
{

    static inline std::vector<name_info_pair> commands_supported =
            {
                    {
                            command_type::Help,
                            "help",
                            {},
                            "provides info about commands"
                    },
                    {
                            command_type::Load,
                            "load",
                            {"branch name", "filename"},
                            "loads a branch from a file and associates it with the name given"
                    },
                    {
                            command_type::Load_1arg,
                            "load",
                            {"branch name"},
                            "loads a branch from the console and associates it with the name given"
                    },
                    {
                            command_type::Save,
                            "save",
                            {"branch name", "filename"},
                            "saves information about the branch in a file; if filename is empty, "
                            "then outputs information on the standard output"
                    },
                    {
                            command_type::Save_1arg,
                            "save",
                            {"branch name", "filename"},
                            "saves information about the branch in a file; if filename is empty"
                            "then outputs information on the standard output"
                    },
                    {
                            command_type::Find,
                            "find",
                            {"branch name", "employee"},
                            "says whether or not a person is employed at a given branch"
                    },
                    {
                            command_type::Num_subordinates,
                            "num_subordinates",
                            {"branch name", "manger"},
                            "gives the number of direct subordinates of manager"
                    },
                    {
                            command_type::Manager,
                            "manager",
                            {"branch name", "employee"},
                            "gives the name of employee's manager in the branch given"
                    },
                    {
                            command_type::Num_employees,
                            "num_employees",
                            {"branch name", "employee"},
                            "gives the number of employees in a given branch"
                    },
                    {
                            command_type::Overloaded,
                            "overloaded",
                            {"branch name"},
                            "gives the number of employees in the branch, who have more than 20 subordinates(direct & indirect)"
                    },
                    {
                            command_type::Join,
                            "join",
                            {"branch_name1", "branch_name2", "result_name"},
                            "merges two branches in a new branch with a given name"
                    },
                    {
                            command_type::Fire,
                            "fire",
                            {"branch name", "employee"},
                            "removes an employee from a branch"
                    },
                    {
                            command_type::Hire,
                            "hire",
                            {"branch name", "employee", "manager"},
                            "sets employee to be a direct subordinate of manager"
                    },
                    {
                            command_type::Salary,
                            "salary",
                            {"branch name", "employee"},
                            "provides information about the salary of an employee of a given branch"
                    },
                    {
                            command_type::Incorporate,
                            "incorporate",
                            {"branch name"},
                            "incorporates a given branch"
                    },
                    {
                            command_type::Modernize,
                            "modernize",
                            {"branch name"},
                            "modernizes a given branch"
                    },
                    {
                            command_type::Exit,
                            "exit",
                            {},
                            "reminds about unsaved files and asks user if they really want to exit"
                    }
            };
    command_type type;
    const std::vector<std::string> arguments;
};


class command_parser
{
    std::string command_name;
    std::vector<std::string> arguments;
public:
    command_parser() = default;
    command parse_line(const std::string& line)
    {
        const char* str = line.c_str();
        skip_white_space(str);
        if(*str) command_name = extract_word(str);
        while(*str)
            arguments.push_back(extract_word(str));
        return inspect();
    }
    void clear()
    {
        command_name.clear();
        arguments.clear();
    }

private:
    command inspect()
    {
        if(command_name == "load" && arguments.size() == 1)
            return command{ command_type::Load_1arg, arguments };
        if(command_name == "save" && arguments.size() == 1)
            return command{ command_type::Save_1arg, arguments };
        /*
        if(command_name == "save" && arguments.size() > 1)
        {
            std::string file_name;
            std::string branch_name = arguments[0];
            for(std::size_t i = 1; i < arguments.size(); ++i)
                file_name += arguments[i];
            arguments.clear();
            arguments.push_back(branch_name);
            arguments.push_back(file_name);
            return command{ command_type::Save, arguments};
        }
        else if(command_name == "save") throw std::invalid_argument("command " +
                                                                    command_name +
                                                                    " wrong format; for more info, type help");
        if(command_name == "load" && arguments.size() > 1)
        {
            std::string file_name;
            std::string branch_name = arguments[0];
            for(std::size_t i = 1; i < arguments.size(); ++i)
                file_name += arguments[i];
            arguments.clear();
            arguments.push_back(branch_name);
            arguments.push_back(file_name);
            return command{ command_type::Load, arguments};
        }
        else if(command_name == "load") throw std::invalid_argument("command " +
                                                                    command_name +
                                                                    " wrong format; for more info, type help");
        -*/
         for(const name_info_pair& elem : command::commands_supported)
        {
            if(elem.name == command_name)
            {
                if(arguments.size() != elem.arguments.size())
                    throw std::invalid_argument("wrong argument count; for use, type \"help\"");
                return command{elem.type, arguments};
            }
        }
        throw std::invalid_argument("command " +
                                    command_name +
                                    " not recognized");
    }

};


#endif //HW2_1_COMMAND_PARSER_H
