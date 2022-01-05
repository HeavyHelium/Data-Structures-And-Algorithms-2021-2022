#include <iostream>
#include <set>
#include "interface.h"
#include <sstream>

int main()
{
    std::stringstream a("one two three four two one");
    std::stringstream b("two one four one one");

    Comparator c;
    ComparisonReport report = c.compare(a, b);
    std::cout << report.commonWords.countOfUniqueWords() << "\n";
    std::cout << report.uniqueWords[0].countOfUniqueWords() << "\n";
    std::cout << report.uniqueWords[1].countOfUniqueWords() << "\n";
    //h.to_multiset();
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

