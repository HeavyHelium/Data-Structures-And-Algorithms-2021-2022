#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

/// rows are houses
/// columns are prices for kth color for nth house
int min_coloring_price(const std::vector<std::vector<int>>& price) {
    if(!price.size() || !price.front().size()) {
        return 0;
    }
    std::vector<std::vector<int>> dp(price.size(),
                                     std::vector<int>(price.front().size()));
    for(int i = 0; i < price.front().size(); ++i) {
        dp[0][i] = price[0][i];
    }
    // for each house:
    for(int i = 1; i < price.size(); ++i) {
        // color it with j
        for(int j = 0; j < price.front().size(); ++j) {
            dp[i][j] = INT_MAX;
            for(int k = 0; k < price.front().size(); ++k) {
                // fing the min of coloring previous house a color different than current j
                if(j != k) {
                    if(price[i][j] + dp[i - 1][k] < dp[i][j]) {
                        dp[i][j] = price[i][j] + dp[i - 1][k];
                    }
                }
            }
        }
    }
    int min = INT_MAX;
    for(int i : price.back()) {
        if(i < min) {
            min = i;
        }
    }
}

int main() {
    std::vector<int> v{ 5, 1, 3, 0, 4, 6, 8 };
    auto print = [](auto elem) { std::cout << elem << " "; };
    std::for_each(v.begin(), v.end(), print);
    std::cout << std::endl;
    std::vector<int> result;
    std::transform(v.begin(), v.end(), std::back_inserter(result), [](int elem) { return 5 * elem; });
    std::for_each(result.begin(), result.end(), print);
    std::cout << std::endl;
    return 0;
}
