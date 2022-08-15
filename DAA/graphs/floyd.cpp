#include <iostream>
#include <algorithm>
#include <climits>
#include <utility>
#include <vector>

static constexpr bool decr = true;
static constexpr long long mod  = 1000000021;


struct edge {
    long long from;
    long long to;
    long long cost;
    friend std::istream& operator>>(std::istream& is, edge& e) {
        is >> e.from >> e.to >> e.cost;
        if(decr) {
            --e.from;
            --e.to;
        }
        return is;
    }
};

struct graph {
    graph(long long n)
            : dp(n, std::vector<long long>(n)),
              ways(n, std::vector<long long>(n, 0)) {
        for(long long i = 0; i < N(); ++i) {
            for(long long j = 0; j < N(); ++j) {
                if(i == j) {
                    dp[i][j] = 0;
                    //ways[i][j] = 1;
                }
                else dp[i][j] = LONG_LONG_MAX;
            }
        }
    }

    void add_edge(const edge& e) {
        long long min1 = std::min(e.cost, dp[e.from][e.to]);
        long long min2 = std::min(e.cost, dp[e.to][e.from]);

        /*if(min1 == dp[e.from][e.to]) {
            ++ways[e.from][e.to];
        } else*/ if(dp[e.from][e.to] > min1) {
            dp[e.from][e.to] = min1;
            ways[e.from][e.to] = 1;
        }

        /*if(min2 == dp[e.to][e.from]) {
            ++ways[e.to][e.from];
        } else*/ if(dp[e.to][e.from] > min2) {
            dp[e.to][e.from] = min2;
            ways[e.to][e.from] = 1;
        }

        ++m;
    }

    long long M() const {
        return m;
    }

    long long N() const {
        return dp.size();
    }

    const std::vector<std::vector<long long>>& floyd_warshall() {
        for(long long k = 0; k < N(); ++k) {
            for(long long i = 0; i < N(); ++i) {
                for(long long j = 0; j < N(); ++j) {
                    if(dp[i][k] != LONG_LONG_MAX &&
                       dp[k][j] != LONG_LONG_MAX) {

                        long long new_d = dp[i][k] + dp[k][j];

                        if(new_d < dp[i][j]) {
                            dp[i][j] = new_d;
                            ways[i][j] = (ways[i][k] * ways[k][j]) % mod;
                        } else if(new_d == dp[i][j]) {
                            ways[i][j] = (ways[i][j] + ways[i][k] * ways[k][j]) % mod;
                        }
                    }
                }
            }
        }
        return ways;
    }

    friend std::istream& operator>>(std::istream& is, graph& g) {
        long long m;
        is >> m;
        edge temp;
        while(m--) {
            is >> temp;
            g.add_edge(temp);
        }
        return is;
    }

private:
    std::vector<std::vector<long long>> dp;
    std::vector<std::vector<long long>> ways;
    long long n;
    long long m{ 0 };
};

std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<long long>>& mat) {
    for(long long i = 0; i < mat.size(); ++i) {
        for(long long j = 0; j < mat[i].size(); ++j) {
            os << mat[i][j] << std::endl;
        }
    }
    return os;
}

void answer_queries(const std::vector<std::vector<long long>>& dp) {
    long long cnt;
    std::cin >> cnt;
    long long i, j;
    while(cnt--) {
        std::cin >> i >> j;
        --i;
        --j;
        std::cout << dp[i][j] << " ";
    }

}

int main() {
    std::cin.sync_with_stdio(false);
    long long n;
    std::cin >> n;

    graph g(n);
    std::cin >> g;

    const std::vector<std::vector<long long>>& res = g.floyd_warshall();

    answer_queries(res);

    return 0;
}