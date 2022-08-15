#include <iostream>
#include <cmath>

namespace my {
// Newton's method
    double sqrt(double n, double epsilon = 0.000001) {
        double x{ n };
        double root;
        int cnt{ 0 };
        for (;;) {
            ++cnt;
            root = 0.5 * (x + n / x);
            if (std::abs(root - x) < epsilon) {
                //std::cout << "Stopped at: " << cnt << std::endl;
                return root;
            }
            x = root;
        }
    }
}

int main() {
    std::cout << my::sqrt(4096) << " vs. " << std::sqrt(4096) << std::endl;
    return 0;
}
