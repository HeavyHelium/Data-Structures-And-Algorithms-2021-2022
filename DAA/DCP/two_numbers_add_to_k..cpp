#include <iostream>
#include <unordered_set>

bool hash_add_to_k(int* array, int size, int k) {
    std::unordered_multiset<int> set;
    for(int i = 0; i < size; ++i) {
        auto iter = set.find(k - array[i]);
        if(iter != set.end()) {
            return true;
        } else {
            set.insert(array[i]);
        }
    }
    return false;
}

int main() {
    int array[] = { 1, 2, 5, 6, 7 };
    int size = sizeof(array) / sizeof(*array);
    std::cout << std::boolalpha << hash_add_to_k(array, size, 4) << std::endl;
    return 0;
}
