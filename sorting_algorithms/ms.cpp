#include <iostream>
#include <algorithm>

void merge(int* arr, int l, int m, int r) {
    int l_size = m - l;
    int r_size = r - m;

    int* l_arr = new int[l_size];
    int* r_arr = new int[r_size];

    for(int i = 0; i < l_size; ++i) {
        l_arr[i] = arr[l + i];
    }

    for(int i = 0; i < r_size; ++i) {
        r_arr[i] = arr[m + i];
    }
    int l_id = 0, r_id = 0;
    int i = l;
    while(l_id < l_size && r_id < r_size) {
        arr[i++] = l_arr[l_id] < r_arr[r_id] ? l_arr[l_id++] : r_arr[r_id++];
    }
    while(l_id < l_size) {
        arr[i++] = l_arr[l_id++];
    }
    while(r_id < r_size) {
        arr[i++] = r_arr[r_id++];
    }
    delete[] l_arr;
    delete[] r_arr;
}

void merge_sort(int* arr, int l, int r) {
    if(l + 1 >= r) return;
    int m = (l + r) / 2;
    merge_sort(arr, l, m);
    merge_sort(arr, m, r);
    merge(arr, l, m, r);
}


int main(){
    int arr[] = { 6, 1, 5, 2, 4, 3, 7, 8, 0, 9 };
    merge_sort(arr, 0, sizeof(arr)/sizeof(*arr));
    for(int elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    return 0;
}
