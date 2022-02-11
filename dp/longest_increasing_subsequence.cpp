#include <iostream>
#include <memory>


struct length_parent {
    int len = 1;
    int parent = -1;
};

/// find the longest increasing subsequence in an array
int find_longest(int* arr, int size) {
    if(!size) return 0;
    std::unique_ptr<length_parent[]> dp = std::make_unique<length_parent[]>(size);
    for(int i = size - 1; i >= 0; --i) {
        int current = 0;
        int p = -1;
        for(int j = i + 1; j < size; ++j) {
            if(arr[j] > arr[i] && dp[j].len > current) {
                current = dp[j].len;
                p = j;
            }
        }
        dp[i].len += current;
        dp[i].parent = p;
    }
    int current_best = dp[0].len;
    int begin = 0;
    for(int i = 1; i < size; ++i) {
        if(dp[i].len > current_best) {
            current_best = dp[i].len;
            begin = i;
        }
    }
    std::cout << "the longest increasing subsequence is: " << std::endl;
    for(int i = begin; i != -1; i = dp[i].parent) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    return current_best;
}

int main() {
    int arr[] = { 50, 0, 3, 2, -1, 10, 7, 40, 80, 90, 10};
    int size = sizeof(arr) / sizeof(*arr);
    std::cout << find_longest(arr, size);
    return 0;
}
