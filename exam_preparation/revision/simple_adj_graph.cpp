#include <iostream>
#include <vector>
#include <list>

struct edge {
    int begin = 0;
    int end = 0;
};

class graph {
    std::vector<std::list<int>> adj_list;
public:
    graph(int n) : adj_list(n) {
        if(n < 0) {
            throw std::invalid_argument("negative counts are not allowed");
        }
    }
    void add_edge(const edge& e) {
        adj_list[e.begin] = e.end;
        adj_list[e.end] = e.begin;
    }
    bool is_cyclic() {
        std::queue<int> q;
        std::vector<bool> visited(adj_list.size(), false);
        q.push(0);
        visited[i] = true;
        while(!q.empty()) {
            int top = q.front();
            q.pop();
            for(int adj : adj_list[top]) {
                if(visited[adj]) {
                    return
                }
            }
        }
    }
};



int main() {
    int n, m;
    std::cout << "type in vertices count: ";
    std::cin >> n;
    std::cout << "type in edge count: ";
    std::cin >> m;
    for(m--) {
        int b, e;
        std::cin >> b >> e;
        add_edge({ b, e });
    }
    graph g{ n, m };
    return 0;
}
