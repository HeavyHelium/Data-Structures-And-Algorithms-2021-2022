#include "van Emde Boas.hpp"


vanEmde_tree::vanEmde_tree(int cnt) {
    if(cnt > 2) {
        int cluster_sz = cluster_size();

        clusters.resize(cluster_sz, nullptr);

        for(int i = 0; i < cluster_sz; ++i) {
            clusters[i] = new vanEmde_tree(cluster_sz);
        }
        summary = new vanEmde_tree(cluster_sz);
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

void vanEmde_tree::insert(int x) {
    if(!empty()) {
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

    } else {
        /// tree is empty here
        m_min = m_max = x; // Theta(1)
        // clusters[high(x)].insert(low(x));
        // don't recursively update cluster
        // summary.insert(high(x))
        // don't recursively update summaries
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
    if(!empty()) {
        if(x < m_min) {
            return m_min;
        } else {
            int i = high(x);
            int j = low(x);
            if(clusters[i]->max() > x) {
                return index(i, clusters[i]->successor(j));
            } else {
                int next_nonempty = summary->successor(i);
                return clusters[next_nonempty]->min();
            }
        }
    } else {
        return None;
    }
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






//c1 c2 c3 c4 // 4 x size 4
//c11 c12      // 8 x size 2

/**

16 + 4 + 1

1 + 2 + 1


*/

