#include <iostream>
#include <vector>

struct info {
    int max_from_here{ 0 };
    bool inclusive{ false };
};

int get_max_sum(int* arr, int size) {
    std::vector<info> dp(size + 1);
    if(!size) {
        return 0;
    } else if(size == 1) {
        return arr[0] > 0 ?
               arr[0] : 0;
    } else {
        dp[size - 1] = arr[size - 1] > 0 ?
                       info{ arr[size - 1], true } :
                       info{ 0, false };
        dp[size] = info{ 0, false };
        for(int i = size - 2; i >= 0; --i) {
            bool incl1{ false };
            bool incl2{ false };
            int case1 = dp[i + 1].max_from_here;
            int case2 = dp[i + 2].max_from_here;
            if(dp[i + 1].inclusive) {
                incl1 = false;
            } else if(arr[i + 1] > 0) {
                incl1 = true;
                case1 += arr[i];
            } else {
                incl1 = false;
            }
            if(arr[i] > 0) {
                case2 += arr[i];
                incl2 = true;
            } else {
                incl2 = false;
            }
            dp[i] = case1 > case2 ? info{ case1, incl1 } :
                                    info{ case2, incl2 };

        }
    }
    return dp[0].max_from_here;
}
// O(n) time, O(1) memory
int get_max_sum_optimized(int* arr, int size) {
    if(!size) {
        return 0;
    } else if(size == 1) {
        return arr[0] > 0 ?
               arr[0] : 0;
    } else {
        info p1 = arr[size - 1] > 0 ?
                       info{ arr[size - 1], true } :
                       info{ 0, false };
        info p2{ 0, false };
        for(int i = size - 2; i >= 0; --i) {
            bool incl1{ false };
            bool incl2{ false };
            int case1 = p1.max_from_here;
            int case2 = p2.max_from_here;
            if(p1.inclusive) {
                incl1 = false;
            } else if(arr[i + 1] > 0) {
                incl1 = true;
                case1 += arr[i];
            } else {
                incl1 = false;
            }
            if(arr[i] > 0) {
                case2 += arr[i];
                incl2 = true;
            } else {
                incl2 = false;
            }
            info temp = case1 > case2 ? info{ case1, incl1 } :
                                        info{ case2, incl2 };
            p2 = p1;
            p1 = temp;
        }
        return p1.max_from_here;
    }
}

int main() {
    int arr1[] = { 5, 5, 10, 40, 50, 35 };
    int arr2[] = { 3, 2, 5, 10, 7 };
    int size1 = sizeof(arr1) / sizeof(*arr1);
    int size2 = sizeof(arr2) / sizeof(*arr2);
    std::cout << get_max_sum_optimized(arr1, size1) << std::endl;
    std::cout << get_max_sum_optimized(arr2, size2) << std::endl;
    return 0;
}
