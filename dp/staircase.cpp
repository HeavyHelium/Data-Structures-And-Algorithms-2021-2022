#include <iostream>
#include <vector>

class solution {
    int stairs_cnt{ 0 };
    std::vector<int> dp;
    const std::vector<int>& options;
public:
    solution(int stairs_cnt, const std::vector<int>& options)
        : stairs_cnt(stairs_cnt), options(options), dp(stairs_cnt + 1) {
    }
    int find_max_number_of_ways() {
        dp[0] = 1;
        for(int i = 1; i <= stairs_cnt; ++i) {
            dp[i] = 0;
            for(int option : options) {
                dp[i] += i - option >= 0 ? dp[i - option] : 0;
            }
        }
        return dp[stairs_cnt];
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<int> options;

    int count_opt, stairs;

    std::cin >> stairs >> count_opt;

    options.resize(count_opt);
    for(int i = 0 ; i < count_opt; ++i) {
       std::cin >> options[i];
    }
    solution s{ stairs, options };
    std::cout << s.find_max_number_of_ways() << std::endl;

    return 0;
}
