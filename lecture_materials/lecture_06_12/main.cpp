#include <iostream>
#include <cassert>
#include "binary_search_tree.h"
int main()
{
    binary_search_tree<int> tree{ 8, 6, 4, 7, 15, 13, 17 };
    tree.print();
    ///std::cout << std::boolalpha << tree.find(15) << std::endl;
    std::cout << "number of nodes: " << tree.size() << std::endl;
    std::cout << "height: " << tree.height() << std::endl;
    std::cout << "is balanced: " << std::boolalpha << tree.balanced() << std::endl;
    std::cout << "is perfectly balanced: " << std::boolalpha << tree.perfectly_balanced() << std::endl;
    std::cout << "check invariant: " << std::boolalpha << tree.check_invariant() << std::endl;

    tree.erase(13);
    assert(tree.size() == 6);
    tree.print();

    return 0;
}
