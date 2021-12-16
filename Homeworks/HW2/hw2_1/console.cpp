#include <iostream>
#include <string>
#include "file_parser.h"
#include "binary_tree.h"
#include "command_parser.h"
#include "input_handler.h"
#include <string>
#define deb_pr

int main()try
{

    input_handler ih;
    ih.getInput();
    //data_parser cp1;
    //command help = cp1.parse_line("help ");
    //std::cout << command::commands_supported[help.type].get_info() << std::endl;
    //cp1.clear();
    //command load = cp1.parse_line("load lonenetz pesho");
    //std::cout << command::commands_supported[load.type].get_info() << std::endl;
    //command c = cp1.parse_line("load");
    //command::commands_supported[c.type];
    //std::cout << c.type;

    //data_parser f1;
    //f1.parse_text("Uspeshnia-gosho\nUspeshnia-pesho\nhaha");
    //std::cout << f1.pairs()[0] << std::endl;
    //std::cout << f1.pairs().size() << std::endl;
    /*
    tree<std::string> branch("Uspeshnia");
    std::cout << branch.size() << std::endl;
    branch.insert_as_child("Uspeshnia", "Pesho");
    branch.insert_as_child("Pesho", "Sherlock");
    branch.insert_as_child("Sherlock", "George");
    std::cout << "Pesho's manager is: " << branch.get_parent_key("Pesho") << std::endl;
    std::cout << "Sherlock's manager is: " << branch.get_parent_key("Sherlock") << std::endl;
    std::cout << "George's manager is: " << branch.get_parent_key("George") << std::endl;
    /*
    int path0[] = {-1};
    int path1[] = {0, -1};
    int path2[] = {0, 0, -1};
    int path3[] = {0, 0, 0, -1};

    branch.insert("Uspeshnia", path0);

    branch.insert("Nasko", path1);
    branch.insert("gosho", path1);
    branch.insert("pesho", path1);
    branch.insert("Шерли", path2);
    branch.insert("George", path3);

    branch.print();
    std::cout << "Sherlock's manager is: " << branch.get_parent_key("Шерли") << std::endl;
    std::cout << "George's manager is: " << branch.get_parent_key("George") << std::endl;
    std::cout << "Шерли има " << branch.number_of_children("Шерли") << " подчинени" << std::endl;
    std::cout << "Успешния има " << branch.number_of_children("Uspeshnia") << " подчинени" << std::endl;
     */
    /*
    for(const auto& pair : f1.pairs())
    {
        //if(branch.find(pair.manager))
    }*/
    return 0;
}
catch(const std::exception& e)
{
    std::cerr << "error: " << e.what();
}