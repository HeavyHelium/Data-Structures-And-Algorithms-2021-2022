#ifndef TABLES_V1_NUMERIC_VALUE_HPP
#define TABLES_V1_NUMERIC_VALUE_HPP
#include "string_helpers.hpp"

union value {
    int i_val;
    double d_val;
};

struct numeric_value {
    type T;
    value V;
    std::string print_value() const;
    void increment();
    void decrement();

    numeric_value& operator+=(const numeric_value& other);
    numeric_value& operator-=(const numeric_value& other);
    numeric_value& operator*=(const numeric_value& other);
    numeric_value& operator/=(const numeric_value& other);

    numeric_value& operator+();
    numeric_value& operator-();
    numeric_value operator+(int);
    numeric_value operator-(int);

    numeric_value operator==(const numeric_value& other) const;
    numeric_value operator!=(const numeric_value& other) const;
    numeric_value operator>(const numeric_value& other) const;
    numeric_value operator<(const numeric_value& other) const;
    operator bool() const;
};

#endif //TABLES_V1_NUMERIC_VALUE_HPP
