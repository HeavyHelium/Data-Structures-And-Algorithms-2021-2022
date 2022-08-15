#include <iostream>
#include <vector>


int container_max_postfix(int capacity, int count, int* weights) {
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
    return dp[capacity - 1][0];
}

int container_max_prefix(int capacity, int count, int* weights) {
    if(!capacity || !count) {
        return 0;
    }
    std::vector<std::vector<int>> dp(capacity, std::vector<int>(count));
    for(int i = 0; i < capacity; ++i) {
        dp[i][0] = weights[i] <= i ? weights[i] : 0;
    }
    for(int j = 1; j < count; ++j) {
        for(int i = 0; i < capacity; ++i) {
            int take = weights[j] < i ? dp[i - weights[j]][j - 1] + weights[j] : 0;
            int drop = dp[j - 1][i];
            dp[i][j] = std::max(take, drop);
        }
    }
    return dp[count - 1][capacity - 1];
}

int main() {
    return 0;
}
