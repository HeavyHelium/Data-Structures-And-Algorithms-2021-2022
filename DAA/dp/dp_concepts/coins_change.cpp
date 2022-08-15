#include <iostream>
#include <vector>
#include <algorithm>

/***
 * mind that here order matters, i.e.:
 *  let sum be 4, coin denominations: { 1, 3, 4, 5 }
 *  for sum 0 -> 1 way, regardless of cou=ins
 *  for sum 1 -> 1 way, using coin with denomination 1
 *  for sum 2 -> using coin with denomination 1, we get ways[2 - 1] = 1
 *  for sum 3 -> using coin with denomination 1, we get ways[3 - 1] = 1
 *            -> using coin with denomination 3, we get ways[3 - 3] = 1
 *            ======================================================= 2
 *  for sum 4 -> using coin with denomination 1, we get ways[4 - 1] = 2
 *            -> using coin with denomination 3, we get ways[4 - 1] = 1
 *            -> using coin with denomination 4, we get ways[4 - 4] = 1
 *            ======================================================= 4
 *  1 1 1 1; 1 3; 4
 *           3 1 is counted as distinct from 3, 1;
 * */

int coins_change(const std::vector<int>& coins, int sum) {
    std::vector<int> dp(sum + 1, 0);
    dp[0] = 1;
    for(int i = 1; i <= sum; ++i) {
        int ways = 0;
        for(int j = 0; j < coins.size(); ++j) {
            if(coins[j] <= i) {
                ways += dp[i - coins[j]];
            }
        }
        dp[i] = ways;
    }
    return dp[sum];
}


int coins_change_opt(const std::vector<int>& coins, int sum) {
    std::vector<int> dp(sum + 1);
    for(int i = 0; i <= sum; ++i) {
        dp[i] = (i == 0);
    }

    for(int j = coins.size() - 1; j >= 0; --j) {
        for(int i = 1; i <= sum; ++i) {
            if(i >= coins[j]) {
                dp[i] += dp[i - coins[j]];
            }
        }
    }
    return dp[sum];
}


int coins_change_opt_no_rep(const std::vector<int>& coins, int sum) {
    std::vector<int> dp(sum + 1);
    for(int i = 0; i <= sum; ++i) {
        dp[i] = (i == 0);
    }

    for(int j = coins.size() - 1; j >= 0; --j) {
        for(int i = sum; i > 0; --i) {
            if(i >= coins[j]) {
                dp[i] += dp[i - coins[j]];
            }
        }
    }
    return dp[sum];
}



int main() {
    std::cout << coins_change_opt({ 1, 3, 4, 5 }, 8) << std::endl;
    std::cout << coins_change_opt_no_rep({ 1, 3, 4, 5 }, 8) << std::endl;
    return 0;
}

/// for sum using coins
