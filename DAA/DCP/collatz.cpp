#include <iostream>
#include <unordered_set>


int collatz_repeat(int n, std::unordered_set<int>& set) {
    if(set.find(n) != set.end()) {
        return n;
    } else {
        set.insert(n);
        return collatz_repeat((n % 2 == 0 ?
                               n / 2 : n * 3 + 1),
                               set);
    }
}



int main() {
    std::unordered_set<int> set;
    int n;

    std::cin >> n;
    std::cout << collatz_repeat(n, set) << '\n';
    return 0;
}
