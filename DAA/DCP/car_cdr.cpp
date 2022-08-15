#include <iostream>


struct pair {
    int first{ 0 };
    int second{ 0 };
};

pair cons(int a, int b) {
    return { a, b };
}

int& car(pair& p) {
    return p.first;
}

const int& car(const pair& p) {
    return p.first;
}

int& cdr(pair& p) {
    return p.second;
}

const int& cdr(const pair& p) {
    return p.second;
}

int main() {
    std::cout << cdr(cons(3, 4)) << std::endl;

    return 0;
}
