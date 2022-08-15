#include <iostream>

long long n, k, a, q;
bool board[1000050];
int strikes[1000050];
int max_can_fit;

void nullify_board() {
    for(int i = 0; i < n; ++i) {
        board[i] = false;
    }
}


bool lie(int id) {
    int can_fit{ 0 };
    int current_empty{ 0 };
    nullify_board();
    for(int i = 0; i <= id; ++i) {
        board[strikes[i]] = true;
    }
    for(int i = 0; i < n; ++i) {
        //std::cout << "current_empty iter: " << current_empty << std::endl;
        if(board[i]) {
            can_fit += (current_empty + 1) / (a + 1);
            current_empty = 0;
          //  std::cout << "can fit iter: " << can_fit << std::endl;
        } else {
            ++current_empty;
        }
    }
    if(!board[n - 1]) {
        ++current_empty;
        can_fit += (current_empty + 1) / (a + 1);
        //std::cout << "here" << can_fit << std::endl;
    }
    //std::cout << "can fit: " << can_fit << std::endl;
    return can_fit < k;
}

int binary_search() {
    int l = 0;
    int r = q;
    int ans = -1;
    while(l <= r) {
        int m = l + (r - l) / 2;
        if(lie(m)) {
            r = m - 1;
            ans = m;
        } else {
            l = m + 1;
        }
    }
    return ans;
}

int main() {
    std::cin >> n >> k >> a >> q;
    for(int i = 0; i < q; ++i) {
        std::cin >> strikes[i];
        --strikes[i];
    }
    max_can_fit = (n + 1) / (a + 1);
    int res = binary_search();
    if(res == -1) {
        std::cout << res << std::endl;
    } else {
        std::cout << res + 1 << std::endl;
    }
    return 0;
}
