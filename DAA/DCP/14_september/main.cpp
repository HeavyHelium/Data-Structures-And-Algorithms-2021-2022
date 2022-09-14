#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>
#include <vector>

/// longest divisible subset/subsequence

struct solution {

    solution(const std::vector<int>& arr) : arr(arr) {
        get();
    }

    bool correct(const std::vector<int>& answer) const {
        if(answer.size() != res.size()) {
            return false;
        }
        for(int i = 0; i < answer.size(); ++i) {
            if(answer[i] != res[i]) {
                return false;
            }
        }
        return true;
    }
private:
    void get() {
        if(!arr.size()) {
            return;
        }
        std::ranges::sort(arr);

        std::vector<int> next_id(arr.size(), -1);
        std::vector<int> lens(arr.size(), 1);

        lens.back() = 1;
        int max_start = arr.size() - 1;

        for(int i = arr.size() - 2; i >= 0; --i) {
            int max_len = -1;
            for(int j = i + 1; j < arr.size(); ++j) {
                if(arr[j] % arr[i] == 0 &&
                   lens[j] > max_len) {
                    max_len = lens[j];
                    next_id[i] = j;
                }
            }
            if(lens[next_id[i]] != -1) {
                lens[i] += lens[next_id[i]];
            }
            if(lens[max_start] < lens[i]) {
                max_start = i;
            }
        }
        for(int i = max_start; i != -1; i = next_id[i]) {
            res.push_back(arr[i]);
        }
    }

    std::vector<int> arr;
    std::vector<int> res;
};


int main() {
    std::vector<int> arr1{ 3, 20, 5, 10, 21 };
    std::vector<int> ans1{ 5, 10, 20 };

    std::vector<int> arr2{ 1, 3, 6, 24 };
    std::vector<int> ans2 = { 1, 3, 6, 24 };

    std::vector<int> arr3{ };
    std::vector<int> ans3 = { };

    std::vector<int> arr4{ 1 };
    std::vector<int> ans4 = { 1 };

    solution s1(arr1);
    solution s2(arr2);
    solution s3(arr3);
    solution s4(arr4);

    assert(s1.correct(ans1));
    assert(s2.correct(ans2));
    assert(s3.correct(ans3));
    assert(s4.correct(ans4));

    return 0;
}
