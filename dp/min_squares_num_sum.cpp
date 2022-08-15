#include <iostream>
#include <vector>
#include <cmath>

int min_squares_num(int n) {
    if(!n) return 1;
    std::vector<int> dp(n);
    dp[0] = 0;
    for(int i = 1; i < n; ++i) {
        dp[i] = i + 1; // more than enough, must change
        for(int j = 1; j <= (int)std::sqrt(i); ++j) {
            if(dp[i - j * j] < dp[i]) {
                dp[i] = dp[i - j * j];
            }
        }
        dp[i] += 1;
    }
    return dp[n - 1];
}


int main() {
    return 0;
}
