#include <iostream>
#include <algorithm>
#include <map>
#include <functional>

static constexpr int MAX_CAPACITY = 2 * 1e5 + 50;

std::ostream& operator<<(std::ostream& os, const std::pair<int, int> p) {
    return os << "(" << p.first << ", " << p.second << ")";
}

/// time complexity: Θ(Q * log(N))

template<int capacity>
struct array {
    int m_size { 0 };
    int buffer[capacity] { 0, };
    friend std::istream& operator>>(std::istream& is, array& a) {
        std::cin >> a.m_size;
        int temp = a.m_size;
        int* iterator = a.buffer;
        while(temp--) {
            is >> *iterator;
            *iterator++ -= 1;
        }
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const array& a) {
        int temp = a.m_size;
        const int* iterator = a.buffer;
        while(temp--) {
            os << *iterator++ << " ";
        }
        return os;
    }
    int size() const {
        return m_size;
    }
};

struct solution {
    using tree_iter = std::map<int, int, std::greater<int>>::iterator;
    static std::map<int, int, std::greater<int>> intervals;
    static array<MAX_CAPACITY> strikes;
    int board_size{ 0 };
    int ship_len { 0 };
    int ship_cnt { 0 };
    int cnt_ships_can_fit(int cap) const {
        return (cap + 1) / (ship_len + 1);
    }
    int lie_id() {
        int ships_can_fit = cnt_ships_can_fit(board_size);
        intervals.insert({ 0, board_size - 1 });
        if(ships_can_fit < ship_cnt) {
            return -1;
        }
        for(int i = 0; i < strikes.size(); ++i) {
            const int current = strikes.buffer[i];
            //std::cout << "current: " << current << std::endl;
            std::map<int, int>::iterator lower = intervals.upper_bound(current);
            if(lower == intervals.end())
                --lower;
           // std::cout << "lb: " << *lower << std::endl;
            if(lower != intervals.end()) {
                    if(lower->first == current) {
                        if(lower->first + 1 > lower -> second) {
              //              std::cout << *lower << " deleted\n";
                            ships_can_fit -= cnt_ships_can_fit(lower->second - lower->first + 1);
                            intervals.erase(lower);
                        } else {
                            int new_beg = lower->first + 1;
                            int end = lower->second;
                            ships_can_fit -= cnt_ships_can_fit(lower->second - lower->first + 1);
                            intervals.erase(lower);
                            intervals.insert({ new_beg, end });
                            ships_can_fit += cnt_ships_can_fit(end - new_beg + 1);
                //           std::cout << *lower << " to " << "(" << new_beg << ", " << end << ")\n";
                        }
                    } else if(lower->second == current) {
                  //      std::cout << *lower;
                        ships_can_fit -= cnt_ships_can_fit(lower->second - lower->first + 1);
                        ships_can_fit += cnt_ships_can_fit(lower->second - lower->first);
                        lower->second--;
                    //    std::cout << " to " << "(" << *lower << std::endl;
                    } else { // it's somewhere in the middle
                        ships_can_fit -= cnt_ships_can_fit(lower->second - lower->first + 1);
                        int r_beg = current + 1;
                        int r_end = lower->second;
                        lower->second = current - 1;
                        intervals.insert({ r_beg, r_end });
                        ships_can_fit += cnt_ships_can_fit(lower->second - lower->first + 1);
                        ships_can_fit += cnt_ships_can_fit(r_end - r_beg + 1);
                      //  std::cout << *lower << " and " << "(" << r_beg << ", " << r_end << ")" << " by " << current << std::endl;
                    }
                } else {
                   std::cout << "never should've reached this\n";
                    return -1;
                }
                //std::cout << "ships can fit: " << ships_can_fit << std::endl;
                if(ships_can_fit < ship_cnt) {
                    return i + 1;
                }
        }
        return -1;
    }
    void observe_splitting() {
        int ships_can_fit = cnt_ships_can_fit(board_size);
        intervals.insert({ 0, board_size - 1 });
        for(int i = 0; i < strikes.size(); ++i) {
            const int current = strikes.buffer[i];
            std::cout << "current: " << current << std::endl;
            std::map<int, int>::iterator lower = intervals.upper_bound(current);
            if(lower == intervals.end())
                --lower;
            std::cout << "lb: " << *lower << std::endl;
            if(lower != intervals.end()) {
                if(lower->first == current) {
                    if(lower->first + 1 > lower -> second) {
                        std::cout << *lower << " deleted\n";
                        ships_can_fit -= cnt_ships_can_fit(lower->second - lower->first + 1);
                        intervals.erase(lower);
                    } else {
                        int new_beg = lower->first + 1;
                        int end = lower->second;
                        ships_can_fit -= cnt_ships_can_fit(lower->second - lower->first + 1);
                        intervals.erase(lower);
                        intervals.insert({ new_beg, end });
                        ships_can_fit += cnt_ships_can_fit(end - new_beg + 1);
                        std::cout << *lower << " to " << "(" << new_beg << ", " << end << ")\n";
                    }
                } else if(lower->second == current) {
                    std::cout << *lower;
                    ships_can_fit -= cnt_ships_can_fit(lower->second - lower->first + 1);
                    ships_can_fit += cnt_ships_can_fit(lower->second - lower->first);
                    lower->second--;
                    std::cout << " to " << "(" << *lower << std::endl;
                } else { // it's somewhere in the middle
                    ships_can_fit -= cnt_ships_can_fit(lower->second - lower->first + 1);
                    int r_beg = current + 1;
                    int r_end = lower->second;
                    lower->second = current - 1;
                    intervals.insert({ r_beg, r_end });
                    ships_can_fit += cnt_ships_can_fit(lower->second - lower->first + 1);
                    ships_can_fit += cnt_ships_can_fit(r_end - r_beg + 1);
                    std::cout << *lower << " and " << r_beg << ", " << r_end << " by " << current << std::endl;
                }
            } else {
                std::cout << "never should've reached this\n";
            }
            // std::cout << "ships can fit: " << ships_can_fit << std::endl;
        }
    }
    friend std::istream& operator>> (std::istream& is, solution& s) {
        return is >> s.board_size >>
                     s.ship_cnt >>
                     s.ship_len >>
                     strikes;
    }
    friend std::ostream& operator<< (std::ostream& os, const solution& s) {
        return os <<  "Strikes array: " <<
                       strikes << std::endl <<
                       "Current state of spaces: " <<
                       s.board_size << std::endl;
    }
};
std::map<int, int, std::greater<int>> solution::intervals;
array<MAX_CAPACITY> solution::strikes;

int main() try {
#if 1
    std::ios_base::sync_with_stdio(false);
    solution s;
    std::cin >> s;
    std::cout << s.lie_id() << "\n";
#endif
    return 0;
} catch(const std::exception& e) {
    std::cerr << "an error with message occurred: "  <<
                  e.what() << '\n';
    return -1;
}

/*
 * 1 1 1
 * 0
 * 15
 *
 * */

/*
11 3 3
5
2 5 7 4 3
 -> 1
 *
 *
 *
 *
 * 8
4
1
5
5 2 1 4 3
 * */
