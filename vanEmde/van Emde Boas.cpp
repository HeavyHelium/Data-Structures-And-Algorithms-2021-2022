#include "van Emde Boas.hpp"
#include <iostream>

vanEmde_tree::vanEmde_tree(int cnt)
    : m_universe_size(cnt) {
    if(cnt < 0) {
        throw std::invalid_argument("Universe size cannot be negative(In this Universe).");
    }

    if(cnt > 2) {
        int cluster_sz = cluster_size();

        clusters.resize(cluster_sz, nullptr);
        // Let's recurse, shall we :)
        for(int i = 0; i < cluster_sz; ++i) {
            clusters[i] = new vanEmde_tree(cluster_sz);
        }
        summary = new vanEmde_tree(cluster_sz);
    } else {
        m_universe_size = 2;
    }
}

bool vanEmde_tree::empty() const {
    return m_min == None;
}

int vanEmde_tree::min() const {
    return m_min;
}

int vanEmde_tree::max() const {
    return m_max;
}

int vanEmde_tree::universe_size() const {
    return m_universe_size;
}

int vanEmde_tree::size() const {
    return current_size;
}

int vanEmde_tree::cluster_size() const {
    return std::ceil(std::sqrt(m_universe_size));
}

bool vanEmde_tree::in_range(int x) const {
    return x >= 0 && x < universe_size();
}

void vanEmde_tree::insert(int x) {
    if(!in_range(x)) {
        throw std::domain_error("element is not in range");
    }
    insert_rec(x);
}

void vanEmde_tree::insert_rec(int x) {
    if(empty()) {
        m_min = m_max = x; // Theta(1)
        return;
    }
    // In order for invariants of min and max to hold true:
    if(x < m_min) {
        std::swap(x, m_min);
    }
    if(x > m_max) {
        m_max = x;
    }

    if(m_universe_size == 2) {
        return; // no clusters, no summary
    }

    int i = high(x);
    int j = low(x);

    if(clusters[i]->empty()) {
        summary->insert(i); // update summary
    }
    clusters[i]->insert(j);

}

void vanEmde_tree::erase(int x) {
    if(!contains(x)) {
        throw std::invalid_argument("Element is not contained in tree!");
    }
    erase_rec(x);
}

void vanEmde_tree::erase_rec(int x) {
    if(x == m_min && x == m_max) { // single element tree
        m_min = m_max = None;
        return;
    }

    if(universe_size() == 2) {
        if(x == m_min) {
            m_min = m_max;
        } else {
            m_max = m_min;
        }
        return;
    }
    /// There is summary here
    if(x == m_min) { // Find new min
        int i = summary->min(); // first non-empty cluster
        if(i == None) {  // Single element tree
            m_min = m_max = None;
            return;
        }
        m_min = index(i, clusters[i]->min()); // Update new min
    }
    int h = high(x);
    int l = low(x);

    clusters[h]->erase(l);
    if(clusters[h]->empty()) { // update summary
        summary->erase(h);
    }
    // In case we've just deleted the max
    if(x == m_max) {
        if(summary->m_max == None) { //if we've deleted the last
            m_max = m_min;
        } else {
            int i = summary->max();
            m_max = index(i, clusters[i]->max());
        }
    }

}

bool vanEmde_tree::contains(int x) const {
    if(m_min == x || m_max == x) {
        return true;
    }
    if(m_universe_size > 2) {
        return clusters[high(x)]->contains(low(x));
    }
    return false;
}

int vanEmde_tree::successor(int x) const {
    return successor_rec(x);
}

int vanEmde_tree::successor_rec(int x) const {
    if(empty()) {
        return None;
    }
    if(x < m_min) {
        return m_min;
    }
    if(universe_size() == 2 && x < m_max) {
        return m_max;
    }
    if(universe_size() == 2) {
        return None;
    }
    // there is a summary vector
    int i = high(x);
    int j = low(x);

    if(clusters[i]->max() > j) { // successor is in the name cluster
        j = index(i, clusters[i]->successor_rec(j));
    } else { // find next nonempty cluster
        i = summary->successor_rec(i);
        if(i == None) {
            return None;
        }
        j = index(i, clusters[i]->min());
    }
    return index(i, j);
}

int vanEmde_tree::predecessor(int x) const {
    return predecessor_rec(x);
}

int vanEmde_tree::predecessor_rec(int x) const {
    if(empty()) {
        return None;
    }
    if(x > m_max) {
        return m_max;
    }
    if(m_universe_size == 2 && x > m_min) {
        return m_min;
    }
    if(m_universe_size == 2) {
        return None;
    }

    int i = high(x);
    int j = low(x);

    if (clusters[i]->min() != None &&
        clusters[i]->min() < j) {
        j = clusters[i]->predecessor_rec(j);
    } else {
        i = summary->predecessor_rec(i); // find the closest nonempty prev

        if (i == None && m_min < x) { // this is essential since min is not stored recursively
            return m_min;
        }

        if (i == None) return None;
        j = clusters[i]->max();
    }
    return index(i, j);
}

vanEmde_tree::~vanEmde_tree() {
    delete summary;
    for(vanEmde_tree*& cluster : clusters) {
        delete cluster;
    }
}

int vanEmde_tree::high(int x) const {
    return x / cluster_size();
}

int vanEmde_tree::low(int x) const {
    return x % cluster_size();
}

int vanEmde_tree::index(int high, int low) const {
    return high * cluster_size() + low;
}

