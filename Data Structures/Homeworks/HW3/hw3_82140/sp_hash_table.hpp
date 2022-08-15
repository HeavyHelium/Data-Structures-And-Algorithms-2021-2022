#pragma once
#ifndef HASH_TABLE_SP_HASH_TABLE_HPP
#define HASH_TABLE_SP_HASH_TABLE_HPP

#include <utility>
#include <functional>
#include <vector>
#include <forward_list>
#include <initializer_list>

template<typename K, typename V>
struct sp_hash_table {
    typedef std::pair<K, V> Item;
    typedef std::forward_list<Item> Bucket;
    typedef std::vector<Bucket> Table;

    typedef typename Table::iterator table_iterator;
    typedef typename Bucket::iterator bucket_iterator;
    typedef typename Table::const_iterator const_table_iterator;
    typedef typename Bucket::const_iterator const_bucket_iterator;

    static constexpr float MAX_LOAD_FACTOR = 0.7f;
    static constexpr int INITIAL_BUCKET_COUNT = 16;
    static constexpr int REHASH_FACTOR = 2;
    static_assert(INITIAL_BUCKET_COUNT > 0);
    static_assert(REHASH_FACTOR > 1);

    struct iterator {
        friend struct hash_table;
    private:
        table_iterator bucket;
        table_iterator end;
        bucket_iterator element;
        void find_next_valid() {
            while(bucket != end && element == bucket -> end()) {
                ++bucket;
                if(bucket != end)
                    element = bucket->begin();
            }
        }

    public:
        iterator(table_iterator b, bucket_iterator e, table_iterator end)
                : bucket(b), element(e), end(end) {
            find_next_valid();
        }
        iterator& operator++() {
            ++element;
            find_next_valid();
            return *this;
        }
        iterator operator++(int) {
            iterator copy = *this;
            ++element;
            find_next_valid();
            return copy;
        }
        bool operator==(const iterator& other) const {
            return bucket == other.bucket &&
                   element == other.element;
        }
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
        std::pair<const K, V>& operator*() {
            return reinterpret_cast<std::pair<const K, V>&>(*element);
        }
        std::pair<const K, V>* operator->() {
            return &reinterpret_cast<std::pair<const K, V>&>(*element);
        }
    };
    struct const_iterator {
        friend struct hash_table;
    private:
        const_table_iterator bucket;
        const_table_iterator end;
        const_bucket_iterator element;
        void find_next_valid() {
            while(bucket != end && element == bucket -> end()) {
                ++bucket;
                if(bucket != end)
                    element = bucket->begin();
            }
        }

    public:
        const_iterator(const_table_iterator b, const_bucket_iterator e, const_table_iterator end)
                : bucket(b), element(e), end(end) {
            find_next_valid();
        }
        const_iterator& operator++() {
            ++element;
            find_next_valid();
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator copy = *this;
            ++element;
            find_next_valid();
            return copy;
        }
        bool operator==(const const_iterator& other) const {
            return bucket == other.bucket &&
                   element == other.element;
        }
        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }
        const Item& operator*() {
            return *element;
        }
        const Item* operator->() {
            return &*element;
        }
    };

    sp_hash_table(std::initializer_list<Item> lst)
        : sp_hash_table() {
        for(const Item& elem : lst)
            insert(elem);
    }

    sp_hash_table() {
        table.resize(INITIAL_BUCKET_COUNT);
    }
    void resize() {
        Table old_table(table.size() * REHASH_FACTOR);
        old_table.swap(table);
        count = 0;
        for(table_iterator bucket = old_table.begin(); bucket != old_table.end(); ++bucket) {
            for(bucket_iterator element = bucket->begin(); element != bucket->end(); ++element) {
                insert(element->first, element->second);
            }
        }
    }

    int size() const {
        return count;
    }
    int bucket_count() const {
        return table.size();
    }
    const_iterator find(const K& key) const {
        const_iterator e = end();
        for(const_iterator it = begin(); it != e; ++it) {
            if(it->first == key)
                return it;
        }
        return e;
    }
    iterator find(const K& key) {
        iterator e = end();
        for(iterator it = begin(); it != e; ++it) {
            if(it->first == key)
                return it;
        }
        return e;
    }
    bool contains(const K& key) const {
        return find(key) != end();
    }
    bool empty() const {
        return count == 0;
    }
    bool should_resize() const {
        return get_load_factor() > MAX_LOAD_FACTOR;
    }
    int get_index(const K& key) const {
        return std::hash<K>()(key) % bucket_count();
    }
    float get_load_factor() const {
        return count / (float)bucket_count();
    }

    void insert(const K& key, const V& value) {
        if(should_resize())
            resize();
        table_iterator bucket = get_bucket(key);
        for(bucket_iterator it = bucket->begin(); it != bucket->end(); ++it) {
            if(it->first == key) {
                *it = Item{ key, value };
                return;
            }
        }
        bucket->emplace_front(key, value);
        ++count;
    }
    void insert(const Item& item) {
        insert(item.first, item.second);
    }
    void erase(const K& key) {
        table_iterator bucket = get_bucket(key);
        for(bucket_iterator it = bucket->before_begin(); it != bucket->end(); ) {
            bucket_iterator prev = it++;
            if(it != bucket->end() && it->first == key) {
                bucket->erase_after(prev);
                --count;
                return;
            }
        }
    }

    V& operator[](const K& key) {
        table_iterator bucket = get_bucket(key);
        for(bucket_iterator it = bucket->begin(); it != bucket-> end(); ++it) {
            if(it->first == key)
                return it->second;
        }
        if(should_resize()) resize();
        bucket->push_front({ key, V()});
        ++count;
        return bucket->front().second;
    }
    iterator begin() {
        return { table.begin(), table.front().begin(), table.end() };
    }
    iterator end() {
        return { table.end(), table.back().end(), table.end() };
    }
    const_iterator begin() const {
        return { table.begin(), table.front().begin(), table.end() };
    }
    const_iterator end() const {
        return { table.end(), table.back().end(), table.end() };
    }

private:
    table_iterator get_bucket(const K& key) {
        return table.begin() + get_index(key);
    }

    Table table;
    int count = 0;
};

#endif
