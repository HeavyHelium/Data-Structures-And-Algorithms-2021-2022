#include <iostream>
#include <vector>


int container_max(int capacity, int count, int* weights) {
    if(!capacity || !count) {
        return 0;
    }
    std::vector<std::vector<int>> dp(capacity, std::vector<int>(count + 1));
    for(int j = 0; j < count; ++j) {
        dp[0][j] = 0;
    }
    for(int i = 0; i < capacity; ++i) {
        dp[i][count] = 0;
    }
    for(int j = count; j >= 0; --j) {
        for(int i = 1; i < capacity; ++i) {
            int take = i > weights[i] ? weights[j] + dp[i - weights[j]][j + 1] : 0;
            dp[i][j] = std::max(take, dp[i][j + 1]);
        }
    }
    return dp[0][capacity - 1];
}

int main() {
    return 0;
}
