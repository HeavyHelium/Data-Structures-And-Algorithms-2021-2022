#ifndef TASKS_TASK1_E2_H
#define TASKS_TASK1_E2_H

#include <cstddef>
#include <cassert>
#include <iostream>
#include <vector>
#include <stdexcept>

using const_iterator = std::vector<int>::const_iterator;

class matrix {
    static constexpr std::size_t MAX_ROWS = 1'000'000'000;
    static constexpr std::size_t MAX_COLUMNS = 1'000'000'000;
    std::size_t n, m;
public:
    std::vector<std::vector<int>> table;
    matrix(const std::size_t n, const std::size_t m)
            : m(m), n(n) {
        assert(m > 0 && n > 0);
        if(n > MAX_ROWS || m > MAX_COLUMNS) {
            throw std::invalid_argument("invalid arguments");
        }
        table.resize(n);
        for(std::size_t i = 0; i < n; ++i) {
            table[i].resize(m);
            //initialize with zeros
            for(std::size_t j = 0; j < m; ++j) {
                table[i][j] = 0;
            }
        }
    }
    // O(len)
    static bool are_linearly_dependent(const_iterator arr1, const_iterator arr2, std::size_t len) {
        assert(len);
        if(*arr1 == 0 && *arr2 != 0) {
            while(len) {
                if(*arr1 != 0) {
                    return false;
                }
                ++arr1;
                --len;
            }
            //arr1 is the null vector
            return true;
        }
        else if(*arr1 != 0 && *arr2 == 0)
            return are_linearly_dependent(arr2, arr1, len);
        else if(*arr1 && *arr2) {
            double coeff = (double)*arr1 / *arr2;
            ++arr1;
            ++arr2;
            while(--len) {
                if(!*arr1 && !*arr2) {
                    ++arr1;
                    ++arr2;
                    continue;
                }
                if(!*arr1 || !*arr2) {
                    return false;
                }
                if(coeff != (double)*arr1++ / *arr2++) {
                    return false;
                }
            }
            return true;
        }
        else {  // both are zeros
            while(len) {
                if(*(arr1) != *(arr2)) break;
                --len;
                ++arr1;
                ++arr2;
            }
            if(len == 0) {
                return true;
            }
            return are_linearly_dependent(arr1, arr2, len);
        }
    }
    // m * (n - 1) + m * (n - 2) + m * (n - 3) + ... + m * 1 = m * (1 + .. + n - 1) = O(m * n^2)
    bool linear_dependency() const {
        for (std::size_t i = 0; i < n - 1; ++i) {
            for (std::size_t j = i + 1; j < n; ++j) {
                if (are_linearly_dependent(table[i].begin(), table[j].begin(), m)) {
                    std::cout << "i: " << i << ", j: " << j;
                    return true;
                }
            }
        }
        return false;
    }
};

int main() {
    std::vector<int> arr1 = { 1, 0, 0, 0 };
    std::vector<int> arr2 = { 1, 0, 5, 0 };
    std::vector<int> arr3 = { 1, 0, 5, 0 };
    std::size_t len1 = arr1.size();
    std::size_t len2 = arr2.size();
    std::size_t len3 = arr3.size();
    assert(len1 == len2);
    assert(len2 == len3);
    std::cout << std::boolalpha <<  matrix::are_linearly_dependent(arr1.begin(), arr2.begin(), len1) << std::endl;
    std::cout << std::boolalpha <<  matrix::are_linearly_dependent(arr2.begin(), arr3.begin(), len1) << std::endl;

    matrix m1(3, 4);
    m1.table[0] = { 1, 2, 3, 4 };
    m1.table[1] = { 2, 5, 6, 8 };
    m1.table[2] = { 0, 0, 0, 1 };

    matrix m2(3, 4);
    m2.table[0] = { 0, 0, 0, 1 };
    m2.table[1] = { 1, 0, 5, 0 };
    m2.table[2] = { 1, 0, 5, 0 };

    std::cout << "The matrix m1 has ";
    if(!m1.linear_dependency()) {
        std::cout << "no linearly dependent rows.\n";
    }
    else std::cout << " as first pair of linearly dependent rows.\n";

    std::cout << "The matrix m2 has ";
    if(!m2.linear_dependency()) {
        std::cout << "no linearly dependent rows.\n";
    }
    else std::cout << " as first pair of linearly dependent rows.\n";

    return 0;
}

#endif //TASKS_TASK1_E2_H
