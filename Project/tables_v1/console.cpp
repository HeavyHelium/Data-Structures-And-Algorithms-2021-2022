#include "input_handler.hpp"
#include <string>
#include "string_helpers.hpp"
#include "cellname.hpp"
#include "Cell.hpp"
#include <cassert>
#include "table.hpp"

#if 1
int main() try {
    table t1;
    //t1.set({ 1, 1 }, "5 + - 6  * (7 + 4)");
    //t1.set({ 1, 1 }, "1 + 7 / -sum(6*5, and(14), 17) * 4");
    t1.set({1, 1}, "\"42\"");
    t1.set({1, 2}, "\"15\"");
    t1.set({1, 20}, "\"69\"");
    std::cout << t1.count_area({ 1, 1 }, { 1, 20 });
    //assert(c.get_numeric().T == type::Int);
    //assert(c.get_numeric().V.i_val == 0);
    //assert(c.save_value() == "\"42 hello world 42\"");
    char ch;
    std::cin.get(ch);
    return 0;
}
catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return -1;
}



#endif
#if 0
int main() {
   input_handler h;
   h.get_input();

   return 0;
}
#endif