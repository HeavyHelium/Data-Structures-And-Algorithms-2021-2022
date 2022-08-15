#include <iostream>
#include <vector>

// 5 - 9 -> 0 - 4

struct range {
    int begin{ 0 };
    int end{ 0 };
    int len() const {
        return end - begin;
    }
    int get_id(int x) const {
        return x - begin;
    }
};

///@brief works by -> counting occurrences of elements
///                -> making a prefix array of occurrences
///                -> then writing the values onto the original array
void counting_sort(int* array,
                   int size,
                   range r) {
    if(!size) return;
    // 1 2 3 1 0 4 3 2
    std::vector<int> occurrences(r.len(), 0);
    for(int i = 0; i < size; ++i) {
        ++occurrences[r.get_id(array[i])];
    }
    std::vector<int> prefix_sum(r.len(), 0);
    prefix_sum[0] = occurrences[0];
    for(int i = 1; i < prefix_sum.size(); ++i) {
        prefix_sum[i] += prefix_sum[i - 1] + occurrences[i];
    }
    std::vector<int> res(size);
    for(int i = 0; i < size; ++i) {
        res[--prefix_sum[r.get_id(array[i])]] = array[i];
    }
    for(int i = 0; i < size; ++i) {
        array[i] = res[i];
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    int array[] = { 5, 1, 2, 2, 4, 4, 0, 70};
    int size = sizeof(array) / sizeof(*array);
    counting_sort(array, size, { 0, 71 });
    for(int i : array) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
