#include <iostream>
#include "linked_stack.hpp"

int main()
{
    linked_stack<int, box_allocator<int>> st1;
    st1.push(25);
    std::cout << st1.top() << std::endl;
    return 0;
}
