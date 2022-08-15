#include <iostream>

double binary_exp(double b, int e, double acc) {
    if(e < 0) {
        return binary_exp(1 / b, -e, acc);
    }
    if(!e) {
        return acc;
    } else {
        if(e % 2 == 0) {
            binary_exp(b * b, e / 2, acc);
        } else {
            binary_exp(b * b, e / 2, acc * b);
        }
    }
}

int main() {
    std::cout << binary_exp(2, -3, 1);
    return 0;
}
