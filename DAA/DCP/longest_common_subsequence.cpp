#include <iostream>
#include <string>
#include <cassert>
#include <algorithm>
#include <utility>
#include <cstddef>
#include <vector>

/// @brief naive recursive implementation 
std::string lcs(const char* word1, 
                const char* word2) { 
    std::string res{  };
    if(!*word1 || !*word2) { 
        return res;
    }
    std::string res1, res2;
    if(*word1 == *word2) { 
        res.push_back(*word1);
        res += lcs(word1 + 1, 
                   word2 + 1);    
    } else { 
        res1 = lcs(word1 + 1, word2);
        res2 = lcs(word1, word2 + 1);
        if(res1.length() > res2.length()) { 
            res += res1;
        } else { 
            res += res2;
        }
    }
    return res;
}

/// @brief DP-optimized O(n * m) implementation
struct lcs_dp { 
    lcs_dp(const std::string& word1, 
           const std::string& word2) 
           : word1(word1), 
             word2(word2), 
             cache(word1.size(), std::vector<int>(word2.size(), -1)) { 
    }
    int get(int n, int m) { 
        if(n == word1.length() ||
           m == word2.length()) { 
            return 0;
        }
        if(cache[n][m] != -1) { // has already been calculated 
            return cache[n][m];
        }
        if(word1[n] == word2[m]) { 
            return cache[n][m] = 1 + get(n + 1, m + 1);
        }
        return cache[n][m] = std::max(get(n + 1, m), 
                                      get(n, m + 1));
    }
    

private: 
    const std::string& word1;
    const std::string& word2;
    std::vector<std::vector<int>> cache;
};

int main() { 
    std::string answer1{ "eieio" };
    assert(lcs("epidemiologist", "refrigeration") == answer1);
    assert(lcs_dp("epidemiologist", "refrigeration").get(0, 0) == answer1.length());

    return 0;
}