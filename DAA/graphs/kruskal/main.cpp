#include <iostream>
#include <algorithm>
#include <queue>
#include "DisjointSet.hpp"


struct edge {
    int from { 0 };
    int to{ 0 };
    int cost{ 0 };
    bool operator<(const edge& other) const {
        return cost < other.cost;
    }
    friend std::istream& operator>>(std::istream& is, edge& e) {
        return is >> e.from >>
               e.to >> e.cost;
    }
    friend std::ostream& operator<<(std::ostream& os, const edge& e) {
       return os << e.from << " "
                 << e .to  << " "<< e.cost;
    }
};

struct edge_link {
    int to{ 0 };
    int cost{ 0 };

    bool operator<(const edge_link& other) const {
        return cost < other.cost;
    }

    friend std::istream& operator>>(std::istream& is, edge_link& e) {
        return is >> e.to >> e.cost;
    }
};

struct graph {
    graph(int n) : adj(n) {
    }


    void add_edge(const edge& e) {
        adj[e.from].push_back({ e.to, e.cost });
        adj[e.to].push_back({ e.from, e.cost });
        edg.push_back(e);
        ++m;
    }

    int M() const {
        return m;
    }

    int N() const {
        return adj.size();
    }

    /// Time complexity : Theta(m * log(n))
    std::pair<graph, int> kruskal() {
        graph mst(N());
        int cost{ 0 };

        std::sort(edg.begin(), edg.end()); // m * log(n) == m * log(m)

        disjoint_set set(N());

        for(auto& e : edg) { // m * alpha(m)
            int parent_u = set.find(e.from); // alpha(m)
            int parent_v = set.find(e.to); // alpha(m)
            if(parent_u != parent_v) {
                mst.add_edge(e);
                cost += e.cost;;
            }
            set.unite_sets(parent_u, parent_v); // alpha(m)

            if(mst.M() == mst.N() - 1) { // the loop can be broken,
                                         // once there are n - 1 edges in tree
                break;
            }

        }
        return { std::move(mst), cost };
    }



    friend std::istream& operator>>(std::istream& is, graph& g) {
        int edg_cnt;
        is >> edg_cnt;
        edge temp;
        while(edg_cnt--) {
            is >> temp;
            g.add_edge(temp);
        }
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const graph& g) {
        os << "(N, M): " << g.N() << " " << g.M() << std::endl;
        os << "edges: " << std::endl;
        for(const auto& e:  g.edg) {
            os << e << std::endl;
        }
        return os;
    }
private:
    std::vector<std::vector<edge_link>> adj;
    std::vector<edge> edg; /// for Kruskal
    int m{ 0 };
};


int main() {
    int n;
    std::cin >> n;

    graph g(n);
    std::cin >> g;

    auto res = g.kruskal();
    std::cout << "Minimal spanning tree/forest:\n" << res.first;
    std::cout << "==================================\n";
    std::cout << "The total cost is: " << res.second << std::endl;

    return 0;
}

/*
 9
14
7 6 1
8 2 2
6 5 2
0 1 4
2 5 4
8 6 6
2 3 7
7 8 7
0 7 8
1 2 8
3 4 9
5 4 10
7 1 11
3 5 14
 * */


/*
4 5
0 1 10
1 3 15
2 3 4
2 0 6
0 3 5
 */