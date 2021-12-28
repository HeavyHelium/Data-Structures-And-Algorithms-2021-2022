#include "input_handler.h"

void input_handler::getInput()
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
        { std::cout << "error: " << e.what() << "\n\n"; continue; }
        std::cout << "\n";
    }

}

Hierarchy* input_handler::find(const std::string& name)
{
    auto br = branches.find(name);
    if(br == branches.end()) return nullptr;
    return br->second;
}

void input_handler::execute_command(const command& c)
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
            if(!br) throw std::invalid_argument(c.arguments[0] + " is an unknown office!");
            br->find(c.arguments[1]) ?
            (std::cout << c.arguments[1] + " is employed in " + c.arguments[0] + "\n"):
            (std::cout << c.arguments[1] + " is not employed in " + c.arguments[0] + "\n");
            break;
        }
        case command_type::Save:
        {
            std::ofstream oFile(c.arguments[1]);
            auto br = branches.find(c.arguments[0]);
            if(br == branches.end()) throw std::invalid_argument(c.arguments[0] + " is an unknown office!");
            oFile << br->second->print();
            std::cout << "Successfully saved branch "
                      << c.arguments[0] << '\n';
            break;
        }
        case command_type::Save_1arg:
        {
            auto br = branches.find(c.arguments[0]);
            if(br == branches.end()) throw std::invalid_argument(c.arguments[0] + " is an unknown office!");
            std::cout << br->second->print();
            break;
        }
        case command_type::Manager:
        {
            auto br = branches.find(c.arguments[0]);
            if(br == branches.end())
                throw std::invalid_argument(c.arguments[0] + " is an unknown office!");
            if(c.arguments[0] == Hierarchy::boss_name)
                throw std::logic_error(Hierarchy::boss_name + " has no boss!");
            std::string temp = br->second->manager(c.arguments[1]);
            if(!temp.empty())
                std::cout << "The manager of "
                          << c.arguments[1] << " is "
                          << temp
                        << ".\n";
            else throw std::logic_error("There is no " + c.arguments[1] + " in " + c.arguments[0] + ".");
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
            if(!br) throw std::invalid_argument(c.arguments[0] + " is an unknown office!");
            if(c.arguments[1] == c.arguments[2])
                throw std::logic_error("manger cannot be subordinate of themselves.");
            if(br->hire(c.arguments[1], c.arguments[2]))
                std::cout << "Successfully added " + c.arguments[1] + " in branch\n";
            else throw std::logic_error("There is no " + c.arguments[2] + " in " + c.arguments[0] + ".");
            break;
        }
        case command_type::Fire:
        {
            Hierarchy* br = find(c.arguments[0]);
            if(!br) throw std::invalid_argument(c.arguments[0] + " is an unknown office!");
            br->fire(c.arguments[1]);
            std::cout << "Successfully fired " + c.arguments[1] + " from branch\n";
            break;
        }
        case command_type::Num_subordinates:
        {
            Hierarchy* br = find(c.arguments[0]);
            if(!br) throw std::invalid_argument(c.arguments[0] + " is an unknown office!");
            std::cout << c.arguments[1] + " has "
                      << br->num_subordinates(c.arguments[1]) << " subordinates\n";
            break;
        }
        case command_type::Num_employees:
        {
            Hierarchy* br = find(c.arguments[0]);
            if(!br) throw std::invalid_argument(c.arguments[0] + " is an unknown office!");
            std::cout << c.arguments[0] + " has "
                      << br->num_employees() << " people employed\n";
            break;
        }
        case command_type::Longest_chain:
        {
            Hierarchy* br = find(c.arguments[0]);
            if(!br) throw std::invalid_argument(c.arguments[0] + " is an unknown office!");
            std::cout << c.arguments[0] + "'s longest chain has length: "
                      << br->longest_chain() << "\n";
            break;
        }
        case command_type::Salary:
        {
            Hierarchy* br = find(c.arguments[0]);
            if(!br) throw std::invalid_argument(c.arguments[0] + " is an unknown office!");
            int salary = br->getSalary(c.arguments[1]);
            if(salary == -1)
                throw std::logic_error("There is no " + c.arguments[1] + " in " + c.arguments[0] + ".");
            std::cout << c.arguments[1] + "'s salary is " << salary << std::endl;
            break;
        }
        case command_type::Overloaded:
        {
            Hierarchy* br = find(c.arguments[0]);
            if(!br) throw std::invalid_argument(c.arguments[0] + " is an unknown office!");
            int cnt = br->num_overloaded();
            std::cout << "in " + c.arguments[0] + " there are "
                      << cnt << " overloaded employees.\n";
            break;
        }
        case command_type::Incorporate:
        {
            Hierarchy* br = find(c.arguments[0]);
            if(!br) throw std::invalid_argument(c.arguments[0] + " is an unknown office!");
            br->incorporate();
            std::cout << "Successfully incorporated " + c.arguments[0] + "\n";
            break;
        }
        case command_type::Modernize:
        {
            Hierarchy* br = find(c.arguments[0]);
            if(!br) throw std::invalid_argument(c.arguments[0] + " is an unknown office!");
            br->modernize();
            std::cout << "Successfully modernized " + c.arguments[0] + "!" + "\n";
            break;
        }
        case command_type::Join:
        {
            Hierarchy* br1 = find(c.arguments[0]);
            Hierarchy* br2 = find(c.arguments[1]);
            if(!br1) throw std::invalid_argument(c.arguments[0] + " is an unknown office!");
            if(!br2) throw std::invalid_argument(c.arguments[1] + " is an unknown office!");
            if(find(c.arguments[2]))
                throw std::invalid_argument(c.argumentss[0] + " already exists");
            Hierarchy* resultant = new Hierarchy(br1->join(*br2));
            if(resultant->num_employees() != 0)
            {
                branches.insert({ c.arguments[2], resultant });
                std::cout << "Successfully joined "
                             + c.arguments[0] + " with "
                             + c.arguments[1] + " into "
                             + c.arguments[2] + "!\n";
            }
            break;
        }
        default: std::cout << "not yet implemented/added\n"; break;
    }
}

input_handler::~input_handler()
{
    for(auto& b: branches)
        delete b.second;
}
