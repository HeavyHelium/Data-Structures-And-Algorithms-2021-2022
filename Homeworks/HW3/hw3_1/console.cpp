#include <iostream>
#include <set>
#include "interface.h"

int main()
{
    hash_table<std::string, int> h;
    h.insert({ "hello", 1 });
    h.insert({ "madafaka", 2 });
    h.insert({ "bitch better have my money", 10000000 });
    h.insert({ "douglas adams", 42 });
    h.to_multiset();
    /*
    for(typename hash_table<std::string, int>::const_iterator iter = h.begin(); iter != h.end(); ++iter)
    {
        std::cout << iter->k << ", " << iter->v << std::endl;
    }*/
    /*
    /*
    if(argc != 3)
        std::cerr << "error: wrong argument count\n"
                  << "\t usage: <program path> <filepath1> <filepath2>\n";*/
    //std::string_view v1(argv[0]);
    //std::cout << argv[0] << std::endl;
    return 0;
}

