#include <iostream>
#include <vector>


std::vector<int> get_products(int* array, int size) {
    int product = 1;
    int zeros_count = 0;
    for(int i = 0; i < size; ++i) {
        product *= array[i] ? array[i] : ++zeros_count, 1;
    }
    std::vector<int> resultant(size);
    for(int i = 0; i < size; ++i) {
        if(zeros_count >= 2) {
            resultant[i] = 0;
        } else  if(zeros_count == 1 && array[i]) {
            resultant [i]= 0;
        } else if(zeros_count == 1 && !array[i]) {
            resultant[i] = product;
        } else {
            resultant[i] = array[i] ? product / array[i] : product;
        }
    }
    return resultant;
}

std::vector<int> get_products_no_division(int* array, int size) {
    if(!size) {
        return {};
    }
    std::vector<int> before(size);
    std::vector<int> after(size);
    before[0] = 1;
    after[size - 1] = 1;
    for(int i = 1; i < size; ++i) {
        before[i] = before[i - 1] * array[i - 1];
    }
    for(int i = size - 2; i >= 0; --i) {
        after[i] = after[i + 1] * array[i + 1];
    }
    for(int i = 0; i < size; ++i) {
        before[i] *= after[i];
    }
    return before;
}



int main() {
    int array[] = { 1, 2, 3, 4, 5 };
    int size = sizeof(array) / sizeof(*array);
    auto result = get_products_no_division(array, size);
    for(int i : result) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return 0;
}
