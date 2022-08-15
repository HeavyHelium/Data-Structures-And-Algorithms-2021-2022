#include <iostream>
#include <string>
#include "hash_map.hpp"

int main() {
    hash_table<std::string, int> table;
    table.insert(hash_table<std::string, int>::pair{ "Gosho", 5 });
    table.insert(hash_table<std::string, int>::pair{ "Pesho", 2 });
    table.insert(hash_table<std::string, int>::pair{ "Peshoo", 2 });
    table.insert(hash_table<std::string, int>::pair{ "kokoo", 2 });
    table.insert(hash_table<std::string, int>::pair{ "kokokoo", 16 });
    table.insert(hash_table<std::string, int>::pair{ "proto", 13 });
    table.insert(hash_table<std::string, int>::pair{ "protokahna", 18 });
    table.insert(hash_table<std::string, int>::pair{ "ipsum", 18 });
    table.insert(hash_table<std::string, int>::pair{ "ilorem", 18 });
    table.insert(hash_table<std::string, int>::pair{ "iloren", 18 });
    table.insert(hash_table<std::string, int>::pair{ "iloren1", 18 });
    table.insert(hash_table<std::string, int>::pair{ "iloren2", 18 });
    table.insert(hash_table<std::string, int>::pair{ "iloren3", 18 });
    table.insert(hash_table<std::string, int>::pair{ "iloren4", 18 });
    table.insert(hash_table<std::string, int>::pair{ "iloren5", 18 });
    table.erase("protokahna");
    table.print();
    std::cout << table.load_factor() << " and max is: " << table.max_load_factor() << std::endl;
    return 0;
}
