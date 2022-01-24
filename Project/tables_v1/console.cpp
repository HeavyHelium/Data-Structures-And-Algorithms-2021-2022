#include "input_handler.hpp"
#include <string>
#include "string_helpers.hpp"
#include "cellname.hpp"
#include "Cell.hpp"

#if 1
int main() {
    numeric_value v;
    string_cell str1("42.5");
    std::cout << str1.save_value() << std::endl;
    //int_cell i1(18);
    //double_cell d1(42.5);
    std::cout << str1.get_numeric().V.d_val << std::endl;
    std::cout << str1.get_numeric().V.d_val << std::endl;
    std::cout << +5 << std::endl;
    //std::cout << i1.get_numeric().V.i_val << std::endl;
    //std::cout << d1.get_numeric().V.d_val << std::endl;

    //v.i_val = 25;
   // std::cout << v.i_val << std::endl;
    return 0;
}



#endif
#if 0
int main() {
   input_handler h;
   h.get_input();

   return 0;
}
#endif