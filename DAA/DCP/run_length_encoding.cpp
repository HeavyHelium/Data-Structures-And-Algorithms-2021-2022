#include <iostream>

std::string encode(const std::string& word) {
    if(!word.size()) {
        return {};
    }
    std::string encoded;
    int cnt = 1;
    int i = 1;
    for( ; i < word.size(); ++i) {
        if(word[i] != word[i - 1]) {
            encoded += std::to_string(cnt);
            encoded += word[i - 1];
            cnt = 1;
        } else {
            ++cnt;
        }
    }
    if(word[i] != word[i - 1]) {
            encoded += std::to_string(cnt);
            encoded += word[i - 1];
            cnt = 1;
    }
    return encoded;
}



int main() {
    std::string word = "AAAABBBCCDAA";
    std::cout << encode(word) << std::endl;
    return 0;
}
