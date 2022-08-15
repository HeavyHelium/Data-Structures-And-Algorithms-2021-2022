#include <iostream>


int partition(int* arr, int l, int r) {
    int pivot = arr[r];
    int write = l;
    for(int read = l; read < r; ++read) {
        if(arr[read] < pivot) {
            std::swap(arr[read], arr[write++]);
        }
    }
    std::swap(arr[write], arr[r]);
    return write;
}

void quick_sort(int* arr, int l, int r) {
    if(l >= r) return;
    int p_pos = partition(arr, l, r);
    //std::cout << "p_pos: " << p_pos << std::endl;
    quick_sort(arr, l, p_pos - 1);
    quick_sort(arr, p_pos + 1, r);
}



int main() {
    int arr[] = { 5, 6, 7, 1, 2, 4, 3 };
    quick_sort(arr, 0, 6);
    for(auto elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    return 0;
}
