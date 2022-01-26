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
    std::cout << t1.count() << std::endl;
    t1.set(absolute_cellname(1, 1), "\"42\"");
    t1.print_val(absolute_cellname(1, 1));
    t1.incr(absolute_cellname(1, 1));
    t1.print_val(absolute_cellname(1, 1));
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