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

    vanEmde_tree() = default;

    vanEmde_tree(int cnt);

    vanEmde_tree(const vanEmde_tree& other) = delete;
    void operator=(const vanEmde_tree& other) = delete;

    bool empty() const;
    int size() const;
    bool contains(int x) const;
    int cluster_size() const;
    int universe_size() const;

    void insert(int x);

    int min() const;
    int max() const;

    int successor(int x) const;
    int predecessor(int x) const;

    ~vanEmde_tree();

private:
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