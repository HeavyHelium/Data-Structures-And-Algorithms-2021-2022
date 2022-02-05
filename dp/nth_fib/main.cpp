#include <iostream>
#include <vector>

class fibbonacci {
    std::vector<int> table;
    int current = -1;
public:
    int operator()(int n) {
       while(current < 2) {
           table.push_back(current);
           ++current;
       }
       while(current < n) {
           table.push_back(table[current - 1] + table[current++]);
       }
       return table[current];
    }
    std::size_t count() const {
        return table.size();
    }
};


int main() {
    fibbonacci f;
    std::cout << f(4)  << std::endl;

    return 0;
}
