#pragma once
#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "sp_hash_table.hpp"
#include <cstddef>

///
/// Represents a multiset of words
///
/// If you need to see how it is intended to be used,
/// check out the corresponding unit tests
///
/// Implement all methods of this class
///

struct amount_pair
{
    std::size_t cnt1 = 0;
    std::size_t cnt2 = 0;
};

class WordsMultiset {
    sp_hash_table<std::string, std::size_t> table;
    std::size_t m_total_word_cnt = 0;
public:
	/// Adds times occurrences of word to the container
	///
	/// For example, add("abc") adds the word "abc" once,
	/// while add("abc", 4) adds 4 occurrences.
	void add(const std::string& word, size_t times = 1);

	/// Checks whether word is contained in the container
	bool contains(const std::string& word) const;

	/// Number of occurrences of word
	size_t countOf(const std::string& word) const;

	/// Number of unique words in the container
	size_t countOfUniqueWords() const;

	/// Returns a multiset of all words in the container
	std::multiset<std::string> words() const;
	
	std::size_t total_word_cnt() const;
};

///
/// Results of the comparison of two streams of words
/// DO NOT modify this class
/// If you need to see how it is intended to be used,
/// check out the corresponding unit tests
///
class ComparisonReport {
public:
	/// A multiset of all words that exist in both streams
	WordsMultiset commonWords;

	/// Multisets of words unique to the two streams
	/// The first element of the array contains the words that are unique
	/// to the first stream (a) and the second one -- to the second stream (b)
	WordsMultiset uniqueWords[2];
};

/// 
/// Can be used to compare two streams of words
///
class Comparator {
public:
	ComparisonReport compare(std::istream& a, std::istream& b);

};

struct data_parser {
    bool first = true;
    std::size_t word_cnt1 = 0;
    std::size_t word_cnt2 = 0;
    sp_hash_table<std::string, amount_pair> words;
    void parse(std::istream& stream);
    void add_word(const std::string& word);
    void print() const;
};
#endif