#include "DisjointSet.hpp"

disjoint_set::disjoint_set(int size)
        : parents(size),
          ranks(size, 0),
          set_cnt(size) {
    for(int i = 0; i < size; ++i) {
        parents[i] = i;
    }
}

void disjoint_set::unite_sets(int a, int b) {
    int root_a = find(a);
    int root_b = find(b);

    if(root_a == root_b) { // they are already in the same subset
        return;
    }
    if(ranks[root_a] < ranks[root_b]) { // then attach a's subset to b's subset
        parents[root_a] = root_b;
    } else { // then attach b's subset to a's subset
        parents[root_b] = root_a;
        // if they happen to have the same rank
        // then root_a's rank must increase
        if(ranks[root_a] == ranks[root_b]) {
            ++ranks[root_a];
        }
    }
    --set_cnt;
}

int disjoint_set::find(int a) {
    int root = a;
    // find root of a
    while(parents[root] != root) {
        root = parents[root];
    }
    // path compression
    // ranks need not change
    // they will change in union operation
    while(parents[a] != a) {
        int old_parent = parents[a];
        parents[a] = root;
        a = old_parent;
    }
    return root;
}

int disjoint_set::size() const {
    return parents.size();
}

int disjoint_set::subsets() const {
    return set_cnt;
}
