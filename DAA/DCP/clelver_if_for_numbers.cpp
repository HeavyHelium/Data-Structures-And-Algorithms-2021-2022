#include <iostream>


/*
    Given three 32-bit integers x, y, and b, return x if b is 1 and y if b is 0,
    using only mathematical or bit operations. You can assume b can
    only be 1 or 0.
*/

int get_number(int b, int x, int y) {
    // if b is 1 => y dies and 1 * x = x is returned
    // else b is 0 => x dies and 1 * y is returned
    return b * x + (1 - b) * y;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout << get_number(0, 5, 6) << '\n'; // should return 6
    std::cout << get_number(1, 5, 6) << '\n'; // should return 5

    return 0;
}
