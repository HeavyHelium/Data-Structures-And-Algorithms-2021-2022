#include <cctype>
#include <iostream>
#include <cassert>
#include <set>
#include "interface.h"
#include "hash_map.hpp"

bool WordsMultiset::contains(const std::string &word) const {
    return table.contains(word);
}

void WordsMultiset::add(const std::string &word, size_t times) {
    if(times) {
        auto found = table.find(word);
        if(found) (found -> v += times);
        else table.insert({ word, times });
        m_total_word_cnt += times;
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
    std::multiset<std::string> s;
    for(const auto& b : table)
        for(const auto& p : b)
            for(std::size_t i = 0; i < p.v; ++i)
                s.insert(p.k);
    return s;
}

std::size_t WordsMultiset::total_word_cnt() const {
    return m_total_word_cnt;
}

ComparisonReport Comparator::compare(std::istream& a, std::istream& b) {
    data_parser p1;
    p1.parse(a);
    p1.parse(b);
    //p1.print();
    WordsMultiset common;
    WordsMultiset unique[2];

    for(const auto& b : p1.words)
    {
        for(const auto& w : b){
            common.add(w.k, std::min(w.v.cnt1, w.v.cnt2));
            if(w.v.cnt1 > w.v.cnt2)
                unique[0].add(w.k, w.v.cnt1 - w.v.cnt2);
            else
                unique[1].add(w.k, w.v.cnt2 - w.v.cnt1);
        }
    }
    //std::cout << "d: " << common.total_word_cnt() << ", " << unique[0].total_word_cnt() << ", " << unique[1].total_word_cnt() << std::endl;
    return { common, { unique[0], unique[1] } };
}

void data_parser::parse(std::istream& stream) {
    //static std::size_t calls = 0;
    std::string word;
    while(stream >> word) {
        //std::cout << word << std::endl;
        add_word(word);
        word.clear();
    }
    if(first) first = false;
    //std::cout << word_cnt1 << ", " << word_cnt2 << std::endl;
    //++calls;
}

void data_parser::add_word(const std::string& word) {
    auto found = words.find(word);
    //std::cout << "h: " << word << std::endl;
    //std::cout << "f: " << (bool) found << std::endl;
    if(first) {
        if(found) ++(found -> v.cnt1);
        else {
            //std::cout << "fins\n";
            assert(words.insert({ word, { 1, 0 } }));
            assert(words.find(word));
        }
        ++word_cnt1;
    }
    else {
        if(found) ++(found -> v.cnt2);
        else words.insert({ word, { 0, 1 } });
        ++word_cnt2;
    }
}

void data_parser::print() const {
    for(const auto& b: words){
        for(const auto& elem: b)
            std::cout << elem.k << ", "
                      << elem.v.cnt1 << " vs "
                      << elem.v.cnt2 << std::endl;
    }
}


