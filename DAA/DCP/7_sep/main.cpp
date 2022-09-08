#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
//#include <iomanip>

/** Google
 * You are given an array of non-negative integers.
 * Let's say you start at the beginning of the array
 * and are trying to advance to the end.
 * You can advance at most,
 * the number of steps that you're currently on.
 * Determine whether you can get to the end of the array.
 * */

struct solution {

    solution(std::initializer_list<int> lst)
        : arr(lst) {
    }

    enum {
        BLANK = -1,
        FALSE = 0,
        TRUE = 1,
    };

    bool solve() const {
        std::vector<int> cache(arr.size());
        for(int i = 0; i < arr.size(); ++i) {
            cache[i] = BLANK;
        }
        cache.back() = true;
        return get_to_end(0, cache);
       // std::cout << std::boolalpha << get_to_end(0, cache) << std::endl;
       // std::for_each(cache.begin(), cache.end(), [](int elem){ std::cout << elem << " "; });
       // std::cout << std::endl;
    }

    friend std::istream& operator>>(std::istream& is,
                                    solution& s) {
        int len;
        is >> len;
        s.arr.resize(len);
        for(int i = 0; i < len; ++i) {
            is >> s.arr[i];
        }
        return is;
    }

private:
    bool get_to_end(const int cid,
                    std::vector<int>& cache) const {
       if(cid >= arr.size()) {
           return false;
       }
       if(cache[cid] != BLANK) {
           return cache[cid];
       }

       int beg = std::min(((int)arr.size()) - 1 - cid, arr[cid]);

       for(int i = beg; i; --i) {
           if(get_to_end(cid + i, cache)) {
               return cache[cid] = TRUE;
           }
       }
       return cache[cid] = FALSE;
    }

    std::vector<int> arr;
};


int main() {
    solution s1{ 1, 3, 2, 0, 1 };
    assert(s1.solve() == true);
    solution s2{ 1, 2, 1, 0, 0 };
    assert(s2.solve() == false);

    return 0;
}
