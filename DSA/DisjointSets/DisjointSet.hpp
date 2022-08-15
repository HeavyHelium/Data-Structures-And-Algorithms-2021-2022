#pragma once
#ifndef DISJOINTSET_DISJOINTSET_HPP
#define DISJOINTSET_DISJOINTSET_HPP

#include <vector>

/// Here assumed key-type is integer.
/// If other key-types are required,
/// then a bijective mapping to the unsigned integers is needed

/// TODO: Modify it so using hashing
struct disjoint_set {

    /// @brief sets ranks to be 0, since at this stage elements are all singletons
    ///
    /// sets the parents of nodes to be themselves, since they are singletons
    disjoint_set(int size);
    /// @param a an element belonging to set X
    /// @param b an element belonging to set Y
    /// @brief merges sets X and Y
    void unite_sets(int a, int b);
    /// @param a element in the set
    /// @return the id/root of the set/tree a is located in
    int find(int a);

    /// @return the total count of elements in all subsets
    int size() const;
    int subsets() const;

private:
    std::vector<int> parents;
    std::vector<int> ranks;
    int set_cnt;
};





#endif //DISJOINTSET_DISJOINTSET_HPP
