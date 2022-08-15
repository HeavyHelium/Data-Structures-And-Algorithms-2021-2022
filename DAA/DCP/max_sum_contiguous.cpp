#include <iostream>

int kadane(int* arr, int size) {
    int sum_suffix{ arr[size - 1] };
    int max = sum_suffix;
    for(int i = size - 2; i >= 0; --i) {
        if(sum_suffix > 0) {
            sum_suffix += arr[i];
        } else {
            sum_suffix = arr[i];
        }
        if(sum_suffix > max) {
            max = sum_suffix;
        }
    }
    return max;
}

int main() {
    int arr[] = { 34, -50, 42, 14, -5, 86 };
    int size = sizeof(arr) / sizeof(*arr);
    std::cout << kadane(arr, size) << std::endl;

    return 0;
}
