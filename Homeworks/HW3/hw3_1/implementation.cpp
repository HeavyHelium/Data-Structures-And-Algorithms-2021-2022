#include "interface.h"
#include "hash_map.hpp"
#include <cctype>
#include <iostream>
#include <cassert>
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
    std::multiset<std::string> s;
    for(const auto& b : table)
        for(const auto& p : b)
            for(std::size_t i = 0; i < p.v; ++i)
                s.insert(p.k);
    return s;
}

ComparisonReport Comparator::compare(std::istream& a, std::istream& b) {
    file_parser p1;
    p1.parse(a);
    p1.parse(b);
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
    return { common, { unique[0], unique[1] } };
}


void file_parser::parse(std::istream& stream) {
    static std::size_t calls = 0;
    std::string word;
    char ch;
    //std::cout << "here at least\n";
    while(stream.get(ch))
    {
        //std::cout << "hello\n";
        if(!isspace(ch)) word.push_back(ch);
        else if(!word.empty())
        {
            add_word(word);
            word.clear();
        }
    }
    if(!word.empty())
        add_word(word);

    if(first) first = false;
    ++calls;
}

void file_parser::add_word(const std::string& word) {
    auto found = words.find(word);
    if(first)
    {
        if(found) ++(found -> v.cnt1);
        else words.insert({ word, { 1, 0 } });
    }
    else
    {
        if(found) ++(found -> v.cnt2);
        else words.insert({ word, { 0, 1 } });
    }
}


