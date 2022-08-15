#include <iostream>
#include <utility>

// partition by sign
/// position the negative in the first half of the array
int partition(int* arr, int size) {
    int j = 0;
    for(int i = 0; i < size; ++i) {
        if(arr[i] <= 0) {
            std::swap(arr[i], arr[j]);
            ++j;
        }
    }
    /// j is now the begin index of the first positive integer
    return j;
}

int find_smallest(int* arr, const int size) {
    int i = 0;
    for( ; i < size; ++i) {
        if(std::abs(arr[i]) - 1 < size &&
           arr[std::abs(arr[i] - 1)] > 0) {
                arr[std::abs(arr[i] - 1)] =
                -arr[std::abs(arr[i]) - 1];
           }
    }
    for (i = 0; i < size; i++)
        if (arr[i] > 0)
            return i + 1;

    return size + 1;
}

///  0  1 2 3 4 5
/// -1 -1 0 1 2 3
// 0 1 2
int get_missing(int* arr, const int size) {
    int begin = partition(arr, size);

    return find_smallest(arr + begin, size - begin);
}


int main() {
    int arr[] = { 1, 2, 3 };
    int arr_size = sizeof(arr) / sizeof(*arr);
    int missing = get_missing(arr, arr_size);
    std::cout << missing << std::endl;
    return 0;
}
