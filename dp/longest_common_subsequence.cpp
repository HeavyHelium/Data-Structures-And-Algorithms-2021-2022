#include <iostream>
#include <string>


int longest_common_subsequence(int i,
                              int j,
                              const std::string& str1,
                              const std::string& str2) {
    if(i == str1.size() || j == str2.size()) {
        return 0;
    } else if(str1[i] == str2[j]) {
        return 1 + longest_common_subsequence(i + 1, j + 1,
                                              str1, str2);
    } else return std::max(longest_common_subsequence(i + 1, j,
                                                      str1, str2),
                           longest_common_subsequence(i, j + 1,
                                                     str1, str2));
}

bool is_subsequence(const std::string& word,
                    const std::string& subs) {
    if(subs.length() > word.length()) return false;
    for(int j = 0, i = 0; i < word.length(); ++i) {
        if(j == subs.length()) {
            return true;
        }
        if(word[i] == subs[i]) ++j;
    }
    return false;
}

int main() {
    std::string str1{ "hello" };
    std::string str2{ "mahalashallaou" };
    std::cout << longest_common_subsequence(0, 0, str1, str2) << std::endl;
    int arr[] = { -1, 3, 0, 1, 2, 7, 5 };
    int size = sizeof(arr) / sizeof(*arr);
    //shift_right(arr, arr + 5);
    //shift_right(arr, arr + 5);
    //shift_right(arr, arr + 5);
    insertion_sort_rec(arr, arr + size);
    for(int i: arr) {
        std::cout << i << " ";
    } std::cout << std::endl;
    return 0;
}
