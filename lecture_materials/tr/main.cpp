#include <iostream>
#include "BST.hpp"
#include <utility>
using namespace fmi::sdp;

int main() {
    BST<int> tree1;
    BST<int> tree2;
    tree1 = std::move(tree2);

    return 0;
}
