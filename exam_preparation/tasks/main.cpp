#include <iostream>
#include <forward_list>
#include <cassert>

#include "list.h"

 bool less_than(int a, int b) {
     return a < b;
 }

 bool greater_than(int a, int b) {
     return a > b;
 }

using list = list;
using iterator = list::iterator;
using const_iterator = list::const_iterator;

struct lst_sum {
    list lst;
    int sum = 0;
    std::size_t len = 0;
};


template<typename T>
 lst_sum longest_monotonous_sublist_helper(const list& lst, T predicate) {
     if(lst.empty()) return {};

     const_iterator current_beg = lst.begin();
     std::size_t current_length = 1;
     int current_sum = *current_beg;

     const_iterator best_beg = current_beg;
     const_iterator best_end = ++lst.begin();
     int best_sum = current_sum;
     std::size_t best_length = 1;

     const_iterator fast = lst.begin();
     const_iterator slow = fast++;

     for(; fast != lst.end(); ++fast, ++slow) {
         std::cout << "big loop\n";
         if(!predicate(*fast, *slow)) {
             // current decreasing sublist has ended
             if(best_length < current_length || best_length == current_length && current_sum > best_sum) {
                 best_beg = current_beg;
                 best_end = fast;
                 best_sum = current_sum;
                 best_length = current_length;
             }
             // renew current values
             current_beg = fast;
             current_length = 1;
             current_sum = * current_beg;
         }
         // the numbers are still monotonous
         else {
             ++current_length;
             current_sum += *fast;
         }
     }

     if(best_length < current_length || best_length == current_length && current_sum > best_sum) {
         best_beg = current_beg;
         best_end = fast;
         best_sum = current_sum;
         best_length = current_length;
     }

     return { list(best_beg, best_end), best_sum, best_length };
 }

 list longest_monotonous_sublist(const list& lst) {
     lst_sum incr = longest_monotonous_sublist_helper(lst, greater_than);
     lst_sum dcr = longest_monotonous_sublist_helper(lst, less_than);
     if(incr.len == dcr.len) {
        return incr.sum > dcr.sum ? incr.lst : dcr.lst;
     }
     return incr.len > dcr.len ? incr.lst : dcr.lst;
 }


#if 0
int main() {
    list l{ 1, 2, 3, 4, 5 };
    list l2(l.begin(), l.end());
    for(int elem: l2) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

#endif

#if 1
int main() {
    list lst{ 2, 3, 2, 1, 0, 3, 2 };
    list lst1{ 5, 4, 3, 2, 1 };
    list lst1_1{ 1, 2, 3, 4, 5, 2, 1, 0, -1, -2 };
    list lst2{ 1, 2, 4, 3, 2, 7, 5, 4, 3, 2, 1 };
    list lst3{ 4, 3, 2, 1, 5, 4, 3, 2, 1 };
    list lst4{ 1 };
    list lst5{ 1, 2 };
    list lst6{ 1, 0 };

    list result = longest_monotonous_sublist(lst);

    for(int e : result) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
    /*
    assert( longest_decreasing_sublist(lst) == (list{ 3, 2, 1, 0 }));
    assert( longest_decreasing_sublist(lst1) == (list{ 5, 4, 3, 2, 1 }));
    assert( longest_decreasing_sublist(lst2) == (list{ 7, 5, 4, 3, 2, 1 }));
    assert( longest_decreasing_sublist(lst3) == (list{ 5, 4, 3, 2, 1 }));
    assert( longest_decreasing_sublist(lst4) == (list{ 1 }));
    assert( longest_decreasing_sublist(lst5) == (list{ 2 }));
    assert( longest_decreasing_sublist(lst6) == (list{ 1, 0 }));*/

    return 0;
}
#endif