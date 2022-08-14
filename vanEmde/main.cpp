#include "catch.hpp"
#include <iostream>
#include <stdexcept>
#include "van Emde Boas.hpp"
#include <cassert>

#if 0
#define CATCH_CONFIG_MAIN

#endif



#if 1

int main() try {

    vanEmde_tree tree(16);
    tree.insert(5);
    tree.insert(7);
    tree.insert(12);
    tree.insert(14);
    tree.insert(3);

    assert(tree.contains(5));
    assert(tree.contains(7));
    assert(tree.contains(12));
    assert(tree.contains(14));
    assert(tree.contains(3));

    tree.erase(3);

    assert(tree.contains(5));
    assert(tree.contains(7));
    assert(tree.contains(12));
    assert(tree.contains(14));

    std::cout << std::endl;

  //  tree.insert(5);
//    tree.insert(6);



    return 0;
} catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
}

#endif