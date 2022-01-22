#include "input_handler.hpp"
#include <string>
#include "string_helpers.hpp"

union value {
    int i_val;
    double d_val;
};

struct numeric_value {
    type T;
    value V;
};

class base_cell {
public:
    virtual numeric_value get_numeric() const = 0;
};
/// in case a clear cell operation is added
class empty_cell : public base_cell {
public:
    empty_cell() = default;
    numeric_value get_numeric() const {
        return numeric_value { type::Int,  0 };
    }
};

class string_cell : public base_cell {
    std::string val;
    numeric_value num_value;
public:
    string_cell(const std::string& str)
        : val(str) {
        type T = determine_type(str.c_str());
        num_value.T = T;
        switch (T) {
            case type::String: {
                num_value.V.i_val = 0;
                break;
            }
            case type::Int: {
                num_value.V.i_val = std::stoi(str);
                break;
            }
            case type::Double: {
                num_value.V.d_val = std::stod(str);
                break;
            }
        }
    }
    numeric_value get_numeric() const override {
        return num_value;
    }
};
/*
class int_cell : public base_cell{
    int val;
public:
    int_cell(int val)
        : val(val)
    {}
    numeric_value get_numeric() const {
        return numeric_value{ type::Int, value{ val } };
    }
};

class double_cell : public base_cell {
    double val;
public:
    double_cell(double val)
        : val(val)
    {}
    numeric_value get_numeric() const {
        value v;
        v.d_val = val;
        return { type::Double, v };
    }
};
*/

class expression_cell : public base_cell {
    numeric_value val;

};

#if 1
int main() {
    numeric_value v;
    string_cell str1("42.5");
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