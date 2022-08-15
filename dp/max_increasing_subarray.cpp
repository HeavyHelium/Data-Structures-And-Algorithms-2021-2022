#include <iostream>

int max_increasing_subarray(int* arr, int size) {
    if(!size) {
        std::cout << "the empty array\n";
        return 0;
    }
    int begin = 0;
    int current_begin = 0;
    int current_len{ 1 };
    int max_len{ 1 };
    for(int i = 1; i < size; ++i) {
        if(arr[i] <= arr[i - 1]) {
            begin = i;
            current_len = 1;
            current_begin = i;
        }
        if(current_len > max_len) {
            max_len = current_len;
            begin = i;
        }
    }
    std::cout << "The max increasing subarray is: \n";
    for(int i = begin; i < begin + max_len; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    return max_len;
}


int main() {

    return 0;
}
