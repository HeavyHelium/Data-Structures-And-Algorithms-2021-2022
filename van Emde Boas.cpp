#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <stdexcept>
#include <climits>
#include <cmath>

/// universe spans in range [0, universe_size)
struct vanEmde_tree {

    // could be set to -1 as well
    static constexpr int None = INT_MIN;

    vanEmde_tree() = default;

    vanEmde_tree(int cnt)
        : m_universe_size(cnt) {
        if(cnt > 2) {
            int cluster_sz = cluster_size();

            clusters.resize(cluster_sz, nullptr);

            for(int i = 0; i < cluster_sz; ++i) {
                clusters[i] = new vanEmde_tree(cluster_sz);
            }
            summary = new vanEmde_tree(cluster_sz);
        }
    }

    vanEmde_tree(const vanEmde_tree& other) = delete;
    void operator=(const vanEmde_tree& other) = delete;

    bool empty() const {
        return m_min == None;
    }

    int min() const {
        return m_min;
    }

    int max() const {
        return m_max;
    }


    int universe_size() const {
        return m_universe_size;
    }

    int size() const {
        return current_size;
    }

    int cluster_size() const {
        return std::ceil(std::sqrt(m_universe_size));
    }

    void insert(int x) {
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

    bool contains(int x) const {
        if(m_min == x || m_max == x) {
            return true;
        }
        if(m_universe_size > 2) {
            return clusters[high(x)]->contains(low(x));
        }
        return false;
    }

    int successor(int x) const {
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

    ~vanEmde_tree() {
        delete summary;
        for(vanEmde_tree*& cluster : clusters) {
            delete cluster;
        }
    }


private:

    /// @return given a universe value x,
    /// returns corresponding cluster index
    int high(int x) const {
        return x / cluster_size();
    }

    int low(int x) const {
        return x % cluster_size();
    }

    int index(int high, int low) const {
        return high * cluster_size() + low;
    }

    std::vector<vanEmde_tree*> clusters;
    vanEmde_tree* summary = nullptr;

    int m_max = None;
    int m_min = None;
    int m_universe_size = 0;
    int current_size = 0;
};


int main() try {

    vanEmde_tree tree(16);

    if(not 5 < 7 or 7 < 8) {
        std::cout << "hello" << std::endl;
    }

    return 0;
} catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
}




//c1 c2 c3 c4 // 4 x size 4
//c11 c12      // 8 x size 2

/**

16 + 4 + 1

1 + 2 + 1


*/

