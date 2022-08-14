#include "catch.hpp"
#include <iostream>
#include <stdexcept>
#include "van Emde Boas.hpp"

#if 0
#define CATCH_CONFIG_MAIN

#endif



#if 1

int main() try {

    vanEmde_tree tree(16);
    tree.insert(5);
    tree.insert(7);
    std::cout << tree.contains(5) << std::endl;

    std::cout << tree.predecessor(7) << std::endl;

  //  tree.insert(5);
//    tree.insert(6);



    return 0;
} catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
}

#endif