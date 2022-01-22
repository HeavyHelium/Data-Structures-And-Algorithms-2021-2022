#ifndef TASKS_TASK2_E3_H
#define TASKS_TASK2_E3_H

#include <iostream>
#include <cstddef>
#include <string>
#include <vector>
#include <unordered_map>

bool isContextSymbol(char ch) {
    return ch >= 'a' && ch <= 'z' ||
           ch >= '0' && ch <= '9';
}

bool solution(const char* a, const char* b) {
    // if both are empty, then it's true
    // std::cout << *a << ", " << *b << std::endl;
    if(!*a && !*b) {
        return true;
    }
    // one is empty, the other not, but it is simply '*'
    if(!*a && *b == '*' && *(b + 1) == '\0' ||
       !*b && *a == '*' && *(a + 1) == '\0') {
        return true;
    }
    // if one is empty and the other not, false
    if(!*a || !*b) {
        return false;
    }
    // both are not empty and are context symbols or '?'
    if(isContextSymbol(*a) && isContextSymbol(*b)) {
        if(*a == *b) return solution(++a, ++b);
        else return false;
    }
    if(*a == '?' ||
       *b == '?' &&
       *a != '*' &&
       *b != '*') {
        return solution(++a, ++b);
    }
    // both are not empty and one is '*'
    if(*a == '*') {
        while(*b) {
            if(solution((a + 1), b))
                return true;
            ++b;
        }
        if(solution(++a, b))
            return true;
    }
    // b is the star one
    else {
        while(*a) {
            if(solution(a, b + 1)) {
                return true;
            }
            ++a;
        }
    }
    return false;
}

struct word_equivalent {
    std::string word;
    std::vector<std::string> equivalent;
};

int main() try {
    /*
    for(;;) {
        std::string a;
        std::string b;
        std::cin >> a >> b;
        std::cout << std::boolalpha << solution(a.c_str(), b.c_str()) << std::endl;
    }*/
    std::string word;
    std::vector<word_equivalent> all_my_reg_exes;
    while(std::cin >> word) {
        all_my_reg_exes.push_back({ word });
    }
    std::cin.clear();
    for(int i = 0; i < all_my_reg_exes.size(); ++i) {
        for(int j = 0; j < all_my_reg_exes.size(); ++j) {
            if(solution(all_my_reg_exes[i].word.c_str(), all_my_reg_exes[j].word.c_str())) {
                all_my_reg_exes[i].equivalent.push_back(all_my_reg_exes[j].word);
            }
        }
    }
    for(const word_equivalent& pair : all_my_reg_exes) {
        std::cout << pair.word << ":  ";
        for(const std::string& eq : pair.equivalent) {
            std::cout << eq << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "...Goodbye!\n..press any key to exit the program\n";
    char ch;
    std::cin.get(ch);
}
catch(const std::exception& e) {
    std::cerr << "an unexpected error occurred with message"
              << e.what() << "\n";
    return -1;
}
#if 0
/*
 hello
hello*
alabala??proto
alabala42proto
1*1and1*1
1john1and1garfield1
1garfield1and1friends1
not_similar
*
 * */
#endif

#endif //TASKS_TASK2_E3_H
