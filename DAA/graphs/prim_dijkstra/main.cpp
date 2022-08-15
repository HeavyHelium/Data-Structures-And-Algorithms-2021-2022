#include <iostream>
#include <algorithm>
#include <climits>
#include <queue>


static constexpr bool decr = false;

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
        return cost > other.cost;
    }

    friend std::istream& operator>>(std::istream& is, edge_link& e) {
        return is >> e.to >> e.cost;
    }
};

struct res {
    std::vector<int> d;
    std::vector<int> parents;
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

    res dijkstra(int source) const {
        std::vector<int> d(N(), INT_MAX);
        std::vector<int> parents(N(), -1);

        std::vector<bool> finished(N(), false);
        std::priority_queue<edge_link> q;

        d[source] = 0;
        q.push({ source, d[source] });

        while(!q.empty()) {
            auto top = q.top();
            q.pop();
            if(finished[top.to]) {
                continue;
            }
            int u = top.to;
            int v, w;
            for(const auto& e : adj[top.to]) {
                // relax edges
                v = e.to;
                w = e.cost;
                if(d[u] + w < d[v]) {
                    d[v] = d[u] + w;
                    parents[v] = u;
                    q.push({ v, d[v] });
                }
            }
            finished[u] = true;
        }
        return { std::move(d), std::move(parents) };
    }

    std::pair<graph, int> prim() {
        graph mst(N());
        int cost{ 0 };

        std::vector<int> d(N(), INT_MAX);
        std::vector<int> parents(N(), -1);

        std::vector<bool> in_mst(N(), false);
        std::priority_queue<edge_link> q;

        int start = 0;

        q.push({ start, 0 });
        bool first = true;

        while(!q.empty()) { // at most m times
            auto top = q.top();
            q.pop(); // log m

            if(in_mst[top.to]) {
                continue;
            }
            in_mst[top.to] = true;

            int u = top.to;

            for(auto& v : adj[u]) {
                if(!in_mst[v.to] &&      // if v is not in tree
                    d[v.to] > v.cost) {  // and its distance from tree
                                         // is greater than current
                                         // edge length
                                         // relax the edge
                    d[v.to] = v.cost;
                    q.push({ v.to, v.cost });    // push end vertex in q
                    parents[v.to] = u;   // set its parent to u;
                }
            }

            cost += top.cost;
            if(!first)
                mst.add_edge({ parents[u], u, top.cost });
            first = false;
            if(mst.M() == mst.N() - 1) {
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

    auto res = g.prim();
    std::cout << "Minimal spanning tree/forest:\n" << res.first;
    std::cout << "==================================\n";
    std::cout << "The total cost is: " << res.second << std::endl;

    int d_source;
    std::cin >> d_source;
    auto r = g.dijkstra(d_source);
    std::cout << "Shortest paths: \n";
    for(int i = 0; i < g.N(); ++i) {
        std::cout << "to " << i << ": " << r.d[i] << std::endl;
    }

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