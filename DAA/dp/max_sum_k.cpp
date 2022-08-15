#include <iostream>

int max_sum_k(int* arr, int size, int k) {
    if(k < size) {
        return 0;
    }
    int current_sum = 0;
    int max_sum = 0;
    for(int i = 0 ; i < k; ++i) {
        current_sum += arr[i];
    }
    max_sum = current_sum;
    for(int i = k; i < size - k + 1; ++i) {
        current_sum += -arr[i - k] + arr[i];
        if(current_sum > max_sum) {
            max_sum = current_sum;
        }
    }
    return max_sum;
}


int main() {

    return 0;
}
