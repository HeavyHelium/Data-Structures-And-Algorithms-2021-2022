#include <iostream>
#include <vector>
#include <climits>
// max subarray sum

// 3 5 4 7 2 6 1 8 9

/// Recurrence relation:
/// max(n - 1) = arr[n - 1]
/// max(i) = max(i + 1) > 0 ? max(i + 1) + arr[i] : arr[i]


///@brief subproblems are represented in the form of suffixes
long long kadane(const std::vector<int>& A) {
    const int size = A.size();
    if(!size) {
        return INT_MIN;
    }
    long long starting_at = A[size - 1];
    long long max_so_far = starting_at;
    for(int i = size - 2; i >= 0; --i) {
            starting_at = starting_at > 0 ?
                          starting_at + A[i] : A[i];
            if(starting_at > max_so_far) {
                max_so_far = starting_at;
            }
    }
    return max_so_far;
}


int main() {

    return 0;
}
