#include "command_parser.h"

std::string name_info_pair::get_info() const
{
    std::string info;
    info += name;
    for(const std::string& arg: arguments)
        info += " <" + arg + "> ";
    return "\t" + info + "\n" + "\t\t" + basic_info;
}

command command_parser::parse_line(const std::string& line)
{
    const char* str = line.c_str();
    skip_white_space(str);
    if(*str) command_name = extract_word(str);
    while(*str)
    {
        if((command_name == "load" || command_name == "save") && arguments.size() == 1)
        {
            arguments.emplace_back(str);
            break;
        }
        arguments.push_back(extract_word(str));
    }

    return inspect();
}

void command_parser::clear()
{
    command_name.clear();
    arguments.clear();
}

command command_parser::inspect()
{
    if(command_name == "load" && arguments.size() == 1)
        return command{ command_type::Load_1arg, arguments };
    if(command_name == "save" && arguments.size() == 1)
        return command{ command_type::Save_1arg, arguments };
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