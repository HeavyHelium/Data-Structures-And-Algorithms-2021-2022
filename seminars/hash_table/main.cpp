#include <iostream>
#include "sp_hash_table.hpp"
#include <string>
#include <cassert>

int main() {
    sp_hash_table<std::string, int> t1;
    t1.insert("gosho", 25);

    for(const auto& elem : t1) {
        std::cout << elem.first << std::endl;
    }
    assert(t1.contains("gosho"));

}