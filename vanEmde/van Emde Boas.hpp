#pragma once
#ifndef VAN_EMDE
#define VAN_EMDE

#include <algorithm>
#include <utility>
#include <vector>
#include <stdexcept>
#include <climits>
#include <cmath>


/// @brief universe spans in range [0, universe_size)
struct vanEmde_tree {
    // could be set to -1 as well, doesn't matter much
    static constexpr int None = INT_MIN;
    static constexpr int DefaultUniverse = 16;

    vanEmde_tree(int cnt = DefaultUniverse);

    vanEmde_tree(const vanEmde_tree& other);
    void operator=(vanEmde_tree other);

    bool empty() const;
    int size() const;
    bool contains(int x) const;
    int cluster_size() const;
    int universe_size() const;
    bool in_range(int x) const;

    void insert(int x);
    void erase(int x);

    int min() const;
    int max() const;

    int successor(int x) const;
    int predecessor(int x) const;

    void clear();

    ~vanEmde_tree();

private:

    void insert_rec(int x);
    void erase_rec(int x);

    bool contains_rec(int x) const;

    int successor_rec(int x) const;
    int predecessor_rec(int x) const;

    /// @return given a universe value x,
    /// returns corresponding cluster index
    int high(int x) const;

    int low(int x) const;

    int index(int high, int low) const;

    std::vector<vanEmde_tree*> clusters;
    vanEmde_tree* summary = nullptr;

    int m_max = None;
    int m_min = None;

    int m_universe_size = 0;
    int current_size = 0;
};



#endif