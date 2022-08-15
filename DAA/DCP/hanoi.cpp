#include <iostream>

void hanoi(int n, int from, int spare, int to, int& cnt) {
    if(!n) {
        return;
    }
    hanoi(n - 1, from, to, spare, cnt);
    std::cout << "moved from " << from << " to " << to << std::endl;
    ++cnt;
    hanoi(n - 1, spare, from, to, cnt);
}

int hanoi_wrapped(int n) {
    int cnt{ 0 };
    hanoi(n, 1, 2, 3, cnt);
    return cnt;
}

int main() {
    std::cout << hanoi_wrapped(3) << " moves in total :)" << std::endl;
    return 0;
}
