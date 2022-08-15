#include <iostream>
#include <vector>

// 111
// 1 11
// 11 1
// 1 1 1

/// Catalan numbers diversion

/// string alpha * beta
/// empty -> 1
/// ab -> 1
/// a(bc), (ab)c -> 2 { 1, 2 }
/// ((ab)c)d, (ab)(cd), (a(bc))d, a((bc)d), a(b(cd)) -> 5 { 1, 3 } U { 2, 2 }
/// -> 2 * (1 * 2) + 1 = 5
/// abcde ->  2 *(1 * 5) + 2 * (1 * 2) = 10 + 4 = 14
/// {1, 4}, { 2, 3 }
/// { 1, 5 }, { 2, 4}, {3, 3}

///Explanation:
// when at id i all we write in dp[i] is the ways we got there
/// base cases:
// when we are at id 0, there was really only 1 way to get there
// that was out starting position
// when at id 1, we got there from id 0, no other way was possible

// more generally for id > 1
// there are 2 possible ways we got there ->
    // first is just the ways we got to id - 1, hence taking the
    // char at id - 1 as a single one
    // second is -- if char at id - 2 and char at id - 1 form
    // a number <= 26, then this means that we can also add to our total
    // number the ways to get to id - 2

int ascii_ways_decoding(const char* string, int len) {
    std::vector<int> dp(len + 1);
    dp[0] = 1;
    dp[1] = 1;

    for(int i = 2; i <= len; ++i) {
        dp[i] = dp[i - 1];
        if(string[i - 2] < '2' ||
           (string[i - 2] == '2' &&
            string[i - 1] <= '6')) {
            dp[i] += dp[i - 2];
        }
    }
    return dp[len];
}

// we are generating numbers in a Fibonacci-esce manner
// so a trivial optimization can be made, reducing memory
// to O(1), i.e. just 2 variables

int ascii_ways_decoding_optimized(const char* string, int len) {
    int dp_0 = 1;
    int dp_1 = 1;

    for(int i = 2; i <= len; ++i) {
        int dp_i = dp_1;
        if(string[i - 2] < '2' ||
           (string[i - 2] == '2' &&
            string[i - 1] <= '6')) {
            dp_i += dp_0;
        }
        dp_0 = dp_1;
        dp_1 = dp_i;
    }
    return dp_1;
}

int main() {
    char word[] = { '1', '2', '1', '4' };
    int len = sizeof(word) / sizeof(*word);
    std::cout << ascii_ways_decoding_optimized(word, len) << std::endl;
    return 0;
}
