#include <iostream>
#include <algorithm>

void heapify(int arr[], int n, int i) {
    int largest_index = i;
    int left_child = 2 * i + 1;
    int right_child = left_child + 1;

    if(left_child < n && arr[left_child] > arr[largest_index]) {
        largest_index = left_child;
    }
    if(right_child < n && arr[right_child] > arr[largest_index]) {
        largest_index = right_child;
    }
    if(largest_index != i) {
         std::swap(arr[i], arr[largest_index]);
         heapify(arr, n, largest_index);
    }

}

void buildHeap(int arr[], int n) {
    int first_parent = (n - 1) / 2;
    for(int i = first_parent; i >= 0; --i) {
        heapify(arr, n, i);
    }

    for(int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}


int main() {
    int arr[] = { 5, 4, 2, 3, 6, 7, 1 };
    buildHeap(arr, 7);
    for(int element : arr) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    return 0;
}
