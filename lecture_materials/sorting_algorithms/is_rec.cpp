#include <iostream>

void shift_right(int* begin, int* end) {
    if(begin >= end) {
        return;
    }
    *end = *(end - 1); // at position end we write
                       // whatever is directly before
    shift_right(begin, end - 1);
}

void insert_in_sorted(int* begin, int* end, int elem) {
    if(begin != end && elem >= *begin) {
        insert_in_sorted(begin + 1, end, elem);
    } else {
        shift_right(begin, end);
        begin[0] = elem;
    }
}

void insertion_sort_rec(int* begin, int* end) {
    if(begin + 1 >= end) {
        return;
    }
    insertion_sort_rec(begin, end - 1);
    insert_in_sorted(begin, end - 1, *(end - 1));
}

int main() {
    int arr[] = { 5, 2, 4, 0, 1, 3 };
    int size = sizeof(arr) / sizeof(*arr);

    insertion_sort_rec(arr, arr + size);

    for(int i: arr) {
        std::cout << i << " ";
    } std::cout << std::endl;

    return 0;
}
