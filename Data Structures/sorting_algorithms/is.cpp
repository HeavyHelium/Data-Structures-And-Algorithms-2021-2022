#include <iostream>
#include <algorithm>


void insertion_sort(int* arr, int size) {
    if(!size) return;
    int sorted_size = 1;
    for(int i = sorted_size; i < size; ++i) {
        int iter = sorted_size - 1;
        int current = i;
        while(iter >= 0 && arr[current] < arr[iter]) {
            std::swap(arr[current--], arr[iter]);
            --iter;
        }
        ++sorted_size;
    }
}

int main() {
    int arr[] = { 5, 7, 1, 3, 6, 2, 4 };
    int size = sizeof(arr) / sizeof(*arr);
    insertion_sort(arr, size);
    for(int element : arr) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    return 0;
}

