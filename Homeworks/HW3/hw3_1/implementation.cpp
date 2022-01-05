#include "interface.h"
#include "hash_map.hpp"
#include <cctype>
#include <iostream>
#include <set>

bool WordsMultiset::contains(const std::string &word) const {
    return table.contains(word);
}

void WordsMultiset::add(const std::string &word, size_t times) {
    while(times) {
        auto found = table.find(word);
        if(found) ++(found -> v);
        else table.insert({ word, 1 });
        --times;
    }
}

size_t WordsMultiset::countOf(const std::string &word) const {
    auto found = table.find(word);
    if(found) return found->v;
    return 0;
}

size_t WordsMultiset::countOfUniqueWords() const {
    return table.size();
}

std::multiset<std::string> WordsMultiset::words() const {
    return table.to_multiset();
}

ComparisonReport Comparator::compare(std::istream& a, std::istream& b) {
    file_parser p1, p2;
    p1.parse(a);
    p2.parse(b);
    return {};
}


void file_parser::parse(std::istream& stream) {
    std::string word;
    char ch;
    while(stream.get(ch))
    {
        if(!isspace(ch)) word.push_back(ch);
        else if(!word.empty())
        {
            auto found = words.find(word);
            if(found) ++(found -> v);
            else words.insert({ word, 0 });
            word.clear();
        }
    }
}
