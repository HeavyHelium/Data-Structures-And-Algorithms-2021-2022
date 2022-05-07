#pragma once
#ifndef TABLES_V1_NUMERIC_VALUE_HPP
#define TABLES_V1_NUMERIC_VALUE_HPP
#include "string_helpers.hpp"

union value {
    int i_val;
    double d_val;
};
///@brief the purpose of this class is to hold an int or a double,
/// depending on what the operations that are being applied to numbers are
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
    numeric_value operator!() const;
    operator bool() const;
    static numeric_value sin(const numeric_value& v1);
    static numeric_value cos(const numeric_value& v1);
    static numeric_value pow(const numeric_value& v1, const numeric_value& other);
};

#endif //TABLES_V1_NUMERIC_VALUE_HPP
