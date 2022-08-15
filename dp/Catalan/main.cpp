#include <iostream>
#include <vector>

struct Catalan {
    static constexpr long long BLANK = -1;
    Catalan(int n) : cache(n ? n + 1 : 2, BLANK) {
        cache[0] = cache[1] = 1;
        catalan_rec(n);
    }

    int size() const {
        return cache.size();
    }

    long long get(int i) const {
        return cache[i];
    }

private:
    long long catalan_rec(int n) {
        if(cache[n] != BLANK) {
            return cache[n];
        }
        int l = 0;
        int r = n - 1;
        long long temp = 0;
        while(l < n) {
            temp += (catalan_rec(l) * catalan_rec(r));
            ++l;
            --r;
        }
        return cache[n] = temp;
    }

    std::vector<long long> cache;
};



int main() {
    Catalan calculator(30);
    for(int i = 0; i <= 30; ++i) {
        std::cout << calculator.get(i) << " ";
    } std::cout << std::endl;
    return 0;
}
