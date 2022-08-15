#include "input_handler.hpp"

int main() try {
   input_handler h;
   h.get_input();

   return 0;
} catch(const std::exception& e) {
    std::cerr << "something went terribly wrong: \n"
              << e.what() << '\n';
    return -1;
}