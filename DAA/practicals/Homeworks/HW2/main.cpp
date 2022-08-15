#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <utility>

static const int N_INFINITY = INT_MIN;

struct edge_link {
    int v{ 0 };
    int cost{ 0 };
    friend std::istream& operator>>(std::istream& is,
                                    edge_link& e) {
        return is >> e.v >> e.cost;
    }
};

struct answer {
    int ways;
    int value;
    answer(int ways = 0, int value = -1)
        : ways(ways),
          value(value) {
    }
    friend std::ostream& operator<<(std::ostream& os,
                                    const answer& a) {
        return os << a.value << " " << a.ways;
    }
};

/// directed, weighted graph
struct graph {
    graph(int n) : adj(n) {
    }
    int N() const {
        return adj.size();
    }
    int M() const {
        return m;
    }
    void add_edge(int from,
                  const edge_link& e) {
        adj[from].push_back(e);
        ++incoming_edges[e.v];
        ++m;
    }
    std::vector<int> topological_sort() {
        std::queue<int> ready; // waiting vertices with zero-incoming edges
        std::vector<int> sorted;
        const int n = N();

        for(int i = 0; i < n; ++i) {
            if(!incoming_edges[i]) {
                ready.push(i);
            }
        }

        while(!ready.empty()) {
            const int node = ready.front();
            ready.pop();
            sorted.push_back(node);

            for(const auto& neighbor : adj[node]) {
                --incoming_edges[neighbor.v];

                if(!incoming_edges[neighbor.v]) {
                    ready.push(neighbor.v);
                }
            }
        }

        if(sorted.size() != n) {
            throw std::logic_error("Only DAGs can be topologically sorted!");
        }
        return std::move(sorted);
    }
    answer number_of_longest_paths(const int source,
                                   const int destination) {
        std::vector<int> topo_order = topological_sort();
        const int n = N();

        std::vector<int> d(n, INT_MIN);
        std::vector<int> ways(n, 0);

        int i = 0;
        while(topo_order[i] != source) { // skip vertices before source, they are unreachable
            if(topo_order[i++] == destination) {
                return answer(); // no ways to get there
            }
        }
        // now i is at source
        d[topo_order[i]] = 0; // distance to source vertex is, of course, zero
        ways[topo_order[i]] = 1; // one way to get to source vertex, already there

        while(topo_order[i] != destination) {
            int u = topo_order[i];
            for(auto neighbour : adj[u]) {
                int v = neighbour.v;
                int w = neighbour.cost;
                int relax_calc = d[u] + w;

                if(relax_calc > d[v]) {
                    d[v] = relax_calc;
                    ways[v] = ways[u]; // new, better way, through u
                } else if(relax_calc == d[v]) { // found just as good of a way, through u
                    ways[v] += ways[u];
                } // else nothing better found for now
            }
            ++i;
        }
        return answer{ ways[destination], d[destination] };
    }
    friend std::istream& operator>>(std::istream& is,
                                    graph& g) {
        int m{ 0 };
        int from{ 0 };
        edge_link temp_link;

        is >> m;
        g.incoming_edges.resize(m);

        std::fill(g.incoming_edges.begin(),
                  g.incoming_edges.end(), 0);

        while(m--) {
            is >> from >> temp_link;
            g.add_edge(from, temp_link);
        }
        return is;
    }
private:
    int m{ 0 };
    std::vector<std::vector<edge_link>> adj;
    ///< to trace count of incoming edges,
    /// for the purposes of topological sort
    std::vector<int> incoming_edges;
};



int main() try {
    std::cin.sync_with_stdio(false);
    int n{ 0 };
    std::cin >> n;

    graph g(n);
    std::cin >> g;
    int source{ 0 };
    int destination{ 0 };
    std::cin >> source >> destination;
    auto res = g.number_of_longest_paths(source,
                                         destination);
    std::cout << res << std::endl;

    return 0;
} catch(const std::exception& e) {
    std::cerr << "The following error occurred: "
              << e.what() << std::endl;
}
