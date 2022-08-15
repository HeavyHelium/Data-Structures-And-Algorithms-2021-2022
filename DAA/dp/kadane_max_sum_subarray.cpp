#include <iostream>


long long max_subarray_sum_suffix(int arr[], int n){
    if(!n) {
        return 0;
    }
    long long next_sum = arr[n - 1];
    long long max_sum = arr[n - 1];
    for(int i = n - 2; i >= 0; --i) {
        if(next_sum > 0) {
            next_sum += arr[i];
        } else {
            next_sum = arr[i];
        }
        if(next_sum > max_sum) {
            max_sum = next_sum;
        }
    }
    return max_sum;
}


long long max_subarray_sum_prefix(int arr[], int n){
    if(!n) {
        return 0;
    }
    long long prev_sum = arr[0];
    long long max_sum = arr[0];
    for(int i = 0; i < n; ++i) {
        if(prev_sum > 0) {
            prev_sum += arr[i];
        } else {
            prev_sum = arr[i];
        }
        if(prev_sum > max_sum) {
            max_sum = prev_sum;
        }
    }
    return max_sum;
}

int main() {

    return 0;
}
