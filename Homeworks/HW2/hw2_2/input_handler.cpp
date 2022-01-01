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
    std::cout << "Goodbye!\n...press any key to close the program\n";
    char ch;
    std::cin.get(ch);

}

typename std::vector<input_handler::hierarchy_state>::iterator input_handler::find_message(const std::string& name)
{
    auto iter = find(name);
    if(iter == hierarchies.end())
        std::cerr << "error: " << name + " is an unknown office!\n";
    return iter;
}

void input_handler::execute_command(const command& c) {
    switch (c.type) {
        case command_type::Help: {
            for (const auto &elem: command::commands_supported)
                std::cout << elem.get_info() << std::endl;
            break;
        }
        case command_type::Load: {
            std::string input = (data_parser::file_data(c.arguments[1]));
            Hierarchy *temp = new Hierarchy(input);
            auto iter = find(c.arguments[0]);
            if(iter != hierarchies.end())
            {
                delete iter->hierarchy;
                hierarchies.erase(iter);
            }
            hierarchies.emplace_back(temp, c.arguments[0], true);
            std::cout << "Successfully loaded branch "
                      << c.arguments[0] << "!\n";
            break;
        }
        case command_type::Load_1arg: {
            std::string input = (data_parser::console_data());
            Hierarchy *temp = new Hierarchy(input);
            auto iter = find(c.arguments[0]);
            if(iter != hierarchies.end())
            {
                delete iter->hierarchy;
                hierarchies.erase(iter);
            }
            hierarchies.emplace_back(temp, c.arguments[0]);
            std::cout << "Successfully loaded branch "
                      << c.arguments[0] << "!\n";
            break;
        }
        case command_type::Find: {
            auto iter = find_message(c.arguments[0]);
            if (iter == hierarchies.end()) return;
            iter->hierarchy->find(c.arguments[1]) ?
            (std::cout << c.arguments[1] + " is employed in " + c.arguments[0] + ".\n") :
            (std::cout << c.arguments[1] + " is not employed in " + c.arguments[0] + ".\n");
            break;
        }
        case command_type::Save: {
            std::ofstream oFile(c.arguments[1]);
            auto iter = find_message(c.arguments[0]);
            if (iter == hierarchies.end()) return;
            oFile << iter->hierarchy->print();
            iter->saved = true;
            std::cout << "Successfully saved branch "
                      << c.arguments[0] << ".\n";
            break;
        }
        case command_type::Save_1arg: {
            auto iter = find_message(c.arguments[0]);
            if (iter == hierarchies.end()) return;
            std::cout << iter->hierarchy->print();
            break;
        }
        case command_type::Manager: {
            auto iter = find_message(c.arguments[0]);
            if (iter == hierarchies.end()) return;
            try {
                std::string temp = iter->hierarchy->manager(c.arguments[1]);
                if (!temp.empty())
                    std::cout << "The manager of "
                              << c.arguments[1] << " is "
                              << temp
                              << ".\n";
                else std::cout << Hierarchy::boss_name + " has no boss.\n";
            }
            catch (const std::exception &e) {
                throw std::logic_error(std::string(e.what()) + " in " + c.arguments[0] + ".");
            }
            break;
        }
        case command_type::Exit: {
            for (const hierarchy_state &h: hierarchies) {
                if (!h.saved) {
                    std::cout << h.name + " has been modified, but not saved.\n"
                                 + "Please, enter filename to save it: \n";
                    std::string line;
                    while (std::cout << "> " &&
                           std::getline(std::cin, line)) {
                        const char *str = line.c_str();
                        skip_white_space(str);
                        if (is_line_empty(str)) continue;
                        else break;
                    }
                    std::ofstream oFile(line);
                    oFile << h.hierarchy->print();
                    std::cout << h.name + " saved\n";
                }
            }
                getting_input = false;
                break;
            }
            case command_type::Hire: {
                auto iter = find_message(c.arguments[0]);
                if (iter == hierarchies.end()) return;
                if (c.arguments[1] == c.arguments[2])
                    throw std::logic_error("Manger cannot be subordinate of themselves.");
                if (iter->hierarchy->hire(c.arguments[1], c.arguments[2])) {
                    iter->saved = false;
                    std::cout << "Successfully hired " + c.arguments[1] + " in branch\n";
                } else
                    throw std::logic_error("There is no " + c.arguments[2] + " in " + c.arguments[0] + ".");
                break;
            }
            case command_type::Fire: {
                auto iter = find_message(c.arguments[0]);
                if (iter == hierarchies.end()) return;
                if (iter->hierarchy->fire(c.arguments[1])) {
                    iter->saved = false;
                    std::cout << "Successfully fired " + c.arguments[1] + " from branch.\n";
                } else if (c.arguments[1] == Hierarchy::boss_name)
                    std::cout << "Cannot fire Uspeshnia!\n";
                else
                    throw std::logic_error("There is no " + c.arguments[1] + " in " + c.arguments[0] + ".");
                break;
            }
            case command_type::Num_subordinates: {
                auto iter = find_message(c.arguments[0]);
                if (iter == hierarchies.end()) return;
                int num = iter->hierarchy->num_subordinates(c.arguments[1]);
                if (num != -1)
                    std::cout << c.arguments[1] + " has "
                              << num << " subordinates.\n";
                else
                    throw std::logic_error("There is no " + c.arguments[1] + " in " + c.arguments[0] + ".");
                break;
            }
            case command_type::Num_all_subordinates:
            {
                auto iter = find_message(c.arguments[0]);
                if (iter == hierarchies.end()) return;
                int num = iter->hierarchy->num_all_subordinates(c.arguments[1]);
                if (num != -1)
                    std::cout << c.arguments[1] + " has "
                              << num << " subordinates.\n";
                else
                    throw std::logic_error("There is no " + c.arguments[1] + " in " + c.arguments[0] + ".");
                break;
            }
            case command_type::Num_employees: {
                auto iter = find_message(c.arguments[0]);
                if (iter == hierarchies.end()) return;
                int num = iter->hierarchy->num_employees();
                std::cout << c.arguments[0] + " has "
                          << iter->hierarchy->num_employees() << " employees.\n";
                break;
            }
            case command_type::Longest_chain: {
                auto iter = find_message(c.arguments[0]);
                if (iter == hierarchies.end()) return;
                std::cout << c.arguments[0] + "'s longest chain has length: "
                          << iter->hierarchy->longest_chain() << "\n";
                break;
            }
            case command_type::Salary: {
                auto iter = find_message(c.arguments[0]);
                if (iter == hierarchies.end()) return;
                int salary = iter->hierarchy->getSalary(c.arguments[1]);
                if (salary == -1)
                    throw std::logic_error("There is no " + c.arguments[1] + " in " + c.arguments[0] + ".");
                std::cout << c.arguments[1] + "'s salary is " << salary << " BGN." << std::endl;
                break;
            }
            case command_type::Overloaded: {
                auto iter = find_message(c.arguments[0]);
                if (iter == hierarchies.end()) return;
                int cnt = iter->hierarchy->num_overloaded();
                std::cout << "In " + c.arguments[0] + " there are "
                          << cnt << " overloaded employees.\n";
                break;
            }
            case command_type::Incorporate: {
                auto iter = find_message(c.arguments[0]);
                if (iter == hierarchies.end()) return;
                iter->hierarchy->incorporate();
                iter->saved = false;
                std::cout << "Successfully incorporated " + c.arguments[0] + "!\n";
                break;
            }
            case command_type::Modernize: {
                auto iter = find_message(c.arguments[0]);
                if (iter == hierarchies.end()) return;
                iter->hierarchy->modernize();
                iter->saved = false;
                std::cout << "Successfully modernized " + c.arguments[0] + "!" + "\n";
                break;
            }
            case command_type::Join: {
                auto iter1 = find_message(c.arguments[0]);
                if (iter1 == hierarchies.end()) return;

                auto iter2 = find_message(c.arguments[1]);
                if (iter2 == hierarchies.end()) return;
                Hierarchy* resultant = new Hierarchy(iter1->hierarchy->join(*iter2->hierarchy));
                if (resultant->num_employees() != 0) {
                    auto iter = find(c.arguments[2]);
                    if(iter != hierarchies.end())
                    {
                        delete iter->hierarchy;
                        hierarchies.erase(iter);
                    }
                    hierarchies.emplace_back(resultant, c.arguments[2]);
                    std::cout << "Successfully joined "
                                 + c.arguments[0] + " with "
                                 + c.arguments[1] + " into "
                                 + c.arguments[2] + "!\n";
                }
                break;
            }
            default:
                std::cout << "not yet implemented/added\n";
            break;
        }
}

input_handler::~input_handler()
{
    for(hierarchy_state& h : hierarchies)
        delete h.hierarchy;
}

typename std::vector<input_handler::hierarchy_state>::iterator input_handler::find(const string &name) {
    for(std::vector<input_handler::hierarchy_state>::iterator iter = hierarchies.begin();
        iter != hierarchies.end(); ++iter)
        if(iter->name == name)
            return iter;
    return hierarchies.end();
}


input_handler::hierarchy_state::hierarchy_state(Hierarchy *h, const string &name, bool saved)
    : hierarchy(h), name(name), saved(saved)
{}
