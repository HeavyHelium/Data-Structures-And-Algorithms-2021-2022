#include <iostream>

void to_lower(char& ch) {
    if(ch <= 'Z' && ch >= 'A') {
        ch = ch - 'A' + 'a';
    }
}


void make_them_low(char* str) {
    while(*str) {
        to_lower(*str++);
    }
}

bool is_lower(char ch) {
    return ch <= 'z' && ch >= 'a';
}

int find_number(char* str) {
    make_them_low(str);
    int hash[26]; // could be just the needed ones,
                 // but whatever
    for(int i = 0; i < 26; ++i) {
        hash[i] = 0;
    }

    for(int i = 0; i < 26; ++i) {
        if(is_lower(*str)) {
                std::cout << *str << std::endl;
            ++hash[*str - 'a'];
        }
        ++str;
    }
    for(int i = 0; i < 26; ++i) {
        std::cout << hash[i] << std::endl;
    }
    int a = hash[0] / 2;
    int b = hash[1];
    int i = hash[str['i' - 'a']];
    int n = hash[str['g' - 'a']];
    int g = hash[str['z' - 'a']];
    return std::min(std::max(std::max(std::max(a, b), i), n),
             g);
}

/// BAZINGA -- 2A 1B 1I 1N 1G 1Z
int main() {
    char str[] = "Farmville is game by Zynga. Btw it made them gazillion dollars!";
    std::cout << find_number(str) << std::endl;
    return 0;
}
