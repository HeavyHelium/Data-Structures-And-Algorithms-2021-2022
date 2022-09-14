#include <iostream>
#include <algorithm>

void rotate(int* arr, int size, int k) {
    for(int i = 0; i < k; ++i) {
        for(int i = 0; i < size - 1; ++i) {
            std::swap(arr[i], arr[i + 1]);
        }
    }
}

int main() {
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    int size = sizeof(arr) / sizeof(*arr);
    rotate(arr, size, 3);
    for(int elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    return 0;
}
