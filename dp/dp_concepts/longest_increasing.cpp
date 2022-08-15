#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <utility>

// 3 4 1 2 1 5

/// recurrence relation:
/// lis(i)
///         int curr{ 0 };
///         for(int j = i + 1; j < n - 1; ++j) {
///               if(arr[j] >= arr[j] && lis(j) > curr) {
///                     curr = lis(j);
///                }
///           }
///          ++curr;

int longest_increasing(const std::vector<int>& arr) {
    if(!arr.size()) {
        return 0;
    }
    std::vector<int> lis(arr.size(), 1);
    int max_incr{ 1 };
    for(int i = arr.size() - 2; i >= 0 ;--i) {
        int curr{ 0 };
        for(int j = i + 1; j < arr.size(); ++j) {
            if(arr[i] <= arr[j] && curr < lis[j]) {
                curr = lis[j];
            }
        }
        lis[i] += curr;
        if(max_incr < lis[i]) {
            max_incr = lis[i];
        }
    }
    return max_incr;
}

struct res {
    int len{ 0 };
    int begin{ -1 };
    std::vector<int> parents;

    std::vector<int> get_sequence() const {
        std::vector<int> seq;
        int p = begin;
        while(p != -1) {
            seq.push_back(p);
            p = parents[p];
        }
        return std::move(seq);
    }
    bool is_ok() const {
        return len > 0 &&
               begin >= 0;
    }
};

res longest_increasing_reconstruct(const std::vector<int>& arr) {
    if(!arr.size()) {
        return {  };
    }
    std::vector<int> lis(arr.size(), 1);
    std::vector<int> parents(arr.size(), -1);

    int max_incr{ 1 };
    int begin = arr.size() - 1;

    for(int i = arr.size() - 2; i >= 0 ;--i) {
        int curr{ 0 };
        for(int j = i + 1; j < arr.size(); ++j) {
            if(arr[i] < arr[j] && curr < lis[j]) {
                curr = lis[j];
                parents[i] = j;
            }
        }
        lis[i] += curr;
        if(max_incr < lis[i]) {
            max_incr = lis[i];
            begin = i;
        }
    }
    return { max_incr, begin, std::move(parents) };
}

int main() {
    const std::vector<int> arr{ 3, 4, 1, 2, 1, 5 };

    auto res = longest_increasing_reconstruct(arr);
    std::vector<int> seq = res.get_sequence();

    std::ranges::for_each(seq, [](const int& elem){ std::cout << elem << " "; });
    std::cout << std::endl;

    assert(res.len == seq.size());
    assert(res.len == longest_increasing(arr));
    return 0;
}

