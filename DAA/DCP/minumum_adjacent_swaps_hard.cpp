#include <iostream>

static constexpr int max_capacity = 1E5 + 5;

long long int merge(int* arr, int* temp, int l, int m, int r) {
    long long int inversion_cnt{ 0 };
    int i = l;
    int j = m;
    int k = l;
    while(i < m && j < r) {
        if(arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
            inversion_cnt += (m - i);
        }
    }
    while(i < m) {
        temp[k++] = arr[i++];
    }
    while(j < r) {
        temp[k++] = arr[j++];
    }
    for(int i = l; i < r; ++i) {
        arr[i] = temp[i];
    }
    return inversion_cnt;
}

long long int merge_sort(int* arr, int* temp, int l, int r) {
    int m;
    long long int inversion_cnt{ 0 };
    if(r > l + 1) {
        m = (r + l) / 2;
        inversion_cnt += merge_sort(arr, temp, l, m);
        inversion_cnt += merge_sort(arr, temp, m, r);
        inversion_cnt += merge(arr, temp, l, m, r);
    }
    return inversion_cnt;
}

void read_array(int* arr, int size) {
    for(int i = 0; i < size; ++i) {
        std::cin >> arr[i];
    }
}
void print_array(int* arr, int size) {
    for(int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    } std::cout << std::endl;
}


int main() {
    /*
    int arr[] = { 1, 20, 6, 4, 5 };
    int n = sizeof(arr) / sizeof(arr[0]);
    int temp[n];
    int ans = merge_sort(arr, temp, 0, n);
    std::cout << "Number of inversions is " << ans;*/
    int arr[100005];
    int temp[100005];
    int size;
    std::cin >> size;
    read_array(arr, size);
    std::cout << merge_sort(arr, temp, 0, size) << std::endl;
   // print_array(arr, size);
    return 0;
}
