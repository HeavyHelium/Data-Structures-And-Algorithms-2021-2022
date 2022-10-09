#include <iostream>
#include <algorithm>
#include <cassert>

int get_min_id(int* arr, int high, int low) {
    if(high == low) {
        return high;
    }
    if(arr[low] < arr[high]) {
        return low; // no rotation
    }
    int mid = low + (high - low) / 2;
    /// we're looking for the elem whose predecessor is larger
    if(mid < high && arr[mid + 1] < arr[mid]) {
        return mid + 1;
    }
    if(mid > low && arr[mid] < arr[mid - 1]) {
        return mid;
    }
    if(arr[high] > arr[mid]) { // sorted according to middle
        return get_min_id(arr, low, mid - 1);
    }
    return get_min_id(arr, mid + 1, high);
    /// 3 4 5 6 7 8 0 1 2
    /// 2 3 4 6 5 7 1 0
    /// 7 8 9 1 2 3 4 5 6
}


int main() {
    int arr[] = { 6, 7, 8, 1, 2, 3, 4, 5 };
    int size = sizeof(arr) / sizeof(*arr);
    assert(get_min_id(arr, size - 1, 0) == 3);
    return 0;
}
