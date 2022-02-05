#include <iostream>
#include <typeinfo>



int main() try {
    std::cout << "task1" << std::endl;
    char ch;
    std::cin.get(ch);
    return 0;
} catch(const std::exception& e) {
    std::cerr << "something went terribly wrong" <<
                  typeid(e).name() << ": " <<
                  e.what() << std::endl;
    return -1;
}

