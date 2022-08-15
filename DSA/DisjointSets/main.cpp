#include <iostream>
#include <vector>
#include <cassert>
#include "DisjointSet.hpp"


int main() {
    disjoint_set gosho(7);
    gosho.unite_sets(1, 2);
    gosho.unite_sets(2, 3);
    gosho.unite_sets(4, 5);
    gosho.unite_sets(5, 6);
    gosho.unite_sets(4, 1);
    gosho.unite_sets(0, 2);

    assert(gosho.find(0) == gosho.find(1));
    assert(gosho.find(1) == gosho.find(2));
    assert(gosho.find(3) == gosho.find(4));
    assert(gosho.find(4) == gosho.find(5));
    assert(gosho.find(5) == gosho.find(6));

    assert(gosho.subsets() == 1);

    return 0;
}
