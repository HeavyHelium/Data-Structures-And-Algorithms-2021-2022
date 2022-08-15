#include <iostream>

/// 1 5 2 3 4
/// 1 2 5 3 4
///

int bubble_sort_swaps(int* arr, int size) {
    int swaps_cnt = 0;
    for(int j = 0; j < size - 1; ++j) {
        bool swaps_made = false;
        for(int i = size - 1; i > j; --i) {
            if(arr[i] < arr[i - 1]) {
                std::swap(arr[i], arr[i - 1]);
                swaps_made = true;
                ++swaps_cnt;
            }
        }
        if(!swaps_made) break;
    }
    return swaps_cnt;
}

void read_array(int* arr, int size) {
    for(int i = 0; i < size; ++i) {
        std::cin >> arr[i];
    }
}

int main() {
    /*
    int arr[] = { 7, 2, 6, 4, 5, 1, 10, 9, 3, 8 };
    int size = sizeof(arr) / sizeof(*arr);
    int swaps_made = bubble_sort_swaps(arr, size);
    for(int i : arr) {
        std::cout << i << " ";
    } std::cout << std::endl;
    std::cout << "swaps made: " << swaps_made << std::endl;
    */
    int arr[100000];
    int size;
    std::cin >> size;
    read_array(arr, size);
    std::cout << bubble_sort_swaps(arr, size) << std::endl;
    return 0;
}
