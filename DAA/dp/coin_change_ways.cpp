#include <iostream>
#include <unordered_map>


int ways(int* coins, int coins_cnt, int sum) {
    std::unordered_map<int, int> dp;
    // first is sum, second is ways
    for(int i = 0; i <= sum; ++i) {
        dp.insert({ i, 0 });
    }
    dp[0] = 1;
    for(int j = 0; j < coins_cnt; ++j) {
        for(int i = 1; i <= sum; ++i) {
            if(coins[j] <= i) {
                dp[i] += dp[i - coins[j]];
            }
        }
    }
    return dp[sum];
}

int min_coins(int* coins, int coins_cnt, int sum) {
    std::unordered_map<int, int> dp;
    // first is sum, second is ways
    for(int i = 0; i <= sum; ++i) {
        dp.insert({ i, -1 });
    }
    dp[0] = 0;
    for(int i = 0; i < coins_cnt; ++i) {
        dp[coins[i]] = 1;
    }

    for(int i = 1; i <= sum; ++i) {
        int best = dp[i];
        for(int j = 0; j < coins_cnt; ++j) {
            if(coins[j] <= i &&
               dp[i - coins[j]] >= 0 &&
               (best > dp[i - coins[j]] ||
                best == -1)) {
                    best = dp[i - coins[j]];
                }
            }
            dp[i] = best != -1 ? best + 1 : best;
            //std::cout << "min: " << dp[i] << " for sum " << i << std::endl;
    }
    return dp[sum];
}


int main() {
    int coins[] = { 2, 4 };
    int coins_cnt = sizeof(coins) / sizeof(*coins);
    int sum = 28;
    std::cout << ways(coins, coins_cnt, sum) << std::endl;
    std::cout << min_coins(coins, coins_cnt, sum);
    return 0;
}
