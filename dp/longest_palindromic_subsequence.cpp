#include <iostream>
#include <cstring>
#include <vector>

void longest_palim_helper(const char* word,
                          int begin,
                          int end,
                          std::vector<std::vector<int>>& dp) {
    if(begin > end || dp[begin][end]) {
        return;
    }
    if(begin == end) {
       dp[begin][end] = 1;
    } else if(word[begin] == word[end]) {
       longest_palim_helper(word, begin + 1, end - 1, dp);
       dp[begin][end] = 2 + dp[begin + 1][end - 1];
    } else {
       longest_palim_helper(word, begin + 1, end, dp);
       longest_palim_helper(word, begin, end - 1, dp);
       dp[begin][end] = std::max(dp[begin + 1][end],
                                 dp[begin][end - 1]);
    }
}

int longest_palim_subsequence(const char* word) {
    if(!*word) {
        return 0;
    }
    int begin = 0;
    int len = std::strlen(word);
    int end = len - 1;
    std::vector<std::vector<int>> dp(len);
    for(int i = 0; i < len; ++i) {
        dp[i].resize(len);
        for(int j = 0; j < len; ++j) {
            dp[i][j] = 0;
        }
    }
    longest_palim_helper(word, begin, end, dp);
    return dp[begin][end];
}


int main() {
    const char* word1 = "turboventilator"; // rotator => 7
    const char* word2 = "underqualified"; // deified => 7
    const char* word3 = "character"; // carac => 5
    std::cout << longest_palim_subsequence(word1) << std::endl;
    std::cout << longest_palim_subsequence(word2) << std::endl;
    std::cout << longest_palim_subsequence(word3) << std::endl;
    return 0;
}
