#include <cctype>
#include <iostream>
#include <cassert>
#include <set>
#include "interface.h"

bool WordsMultiset::contains(const std::string &word) const {
    return table.contains(word);
}

void WordsMultiset::add(const std::string &word, size_t times) {
    if(times) {
        auto found = table.find(word);
        if(found != table.end()) (found -> second += times);
        else table.insert(word, times);
        m_total_word_cnt += times;
    }
}

size_t WordsMultiset::countOf(const std::string &word) const {
    auto found = table.find(word);
    if(found != table.end()) return found->second;
    return 0;
}

size_t WordsMultiset::countOfUniqueWords() const {
    return table.size();
}

std::multiset<std::string> WordsMultiset::words() const {
    std::multiset<std::string> s;
    for(auto& elem : table)
        for(std::size_t i = 0; i < elem.second; ++i)
            s.insert(elem.first);
    return s;
}

std::size_t WordsMultiset::total_word_cnt() const {
    return m_total_word_cnt;
}

ComparisonReport Comparator::compare(std::istream& a, std::istream& b) {
    data_parser p1;
    p1.parse(a);
    p1.parse(b);
    WordsMultiset common;
    WordsMultiset unique[2];

    for(const auto& elem : p1.words) {
        common.add(elem.first, std::min(elem.second.cnt1, elem.second.cnt2));
        if(elem.second.cnt1 > elem.second.cnt2)
            unique[0].add(elem.first, elem.second.cnt1 - elem.second.cnt2);
        else
            unique[1].add(elem.first, elem.second.cnt2 - elem.second.cnt1);
    }
    return { common, { unique[0], unique[1] } };
}

void data_parser::parse(std::istream& stream) {
    std::string word;
    while(stream >> word) {
        add_word(word);
        word.clear();
    }
    if(first) first = false;
}

void data_parser::add_word(const std::string& word) {
    auto found = words.find(word);
    auto end = words.end();
    if(first) {
        if(found != end) ++(found -> second.cnt1);
        else {
            words.insert(word, { 1, 0 });
            assert(words.find(word) != end);
        }
        ++word_cnt1;
    }
    else {
        if(found != end) ++(found -> second.cnt2);
        else words.insert(word, { 0, 1 });
        ++word_cnt2;
    }
}

void data_parser::print() const {
    for(const auto& elem : words)
        std::cout << elem.first << ", "
                  << elem.second.cnt1 << " vs "
                  << elem.second.cnt2 << std::endl;
}


