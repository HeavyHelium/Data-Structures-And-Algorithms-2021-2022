#include "input_handler.hpp"
#include <string>
#include "string_helpers.hpp"
#include "cellname.hpp"
#include "Cell.hpp"
#include <cassert>


#if 0
int main() {
    string_cell c("42 hello world 42");
    //assert(c.get_numeric().T == type::Int);
    //assert(c.get_numeric().V.i_val == 0);
    //assert(c.save_value() == "\"42 hello world 42\"");
    return 0;
}



#endif
#if 1
int main() {
   input_handler h;
   h.get_input();

   return 0;
}
#endif