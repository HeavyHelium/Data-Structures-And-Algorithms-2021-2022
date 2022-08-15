#include <iostream>

/*
Given an array of n elements that contains elements
from 0 to n-1, with any of these numbers appearing
any number of times. Find these repeating numbers in
O(n) and using only constant memory space.*/

void find_duplicates(int* arr, int size) {
    for(int i = 0; i < size; ++i) {
        arr[arr[i] % size] += size;
    }
    for(int i = 0; i < size; ++i) {
        if(arr[i] / size >= 2) {
            std::cout << arr[i] % size << " ";
        }
    }
    std::cout << std::endl;
}


int main() {
    int array[] = { 0, 4, 3, 2, 7, 8, 2, 3, 1 };
    int size = sizeof(array) / sizeof(*array);
    find_duplicates(array, size);
    return 0;
}
