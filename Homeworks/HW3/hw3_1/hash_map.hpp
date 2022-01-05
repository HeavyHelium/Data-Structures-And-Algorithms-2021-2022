#pragma once
#ifndef hash_table_HPP
#define hash_table_HPP
#include <functional>
#include <cstddef>
#include <stdexcept>
#include <vector>
#include <forward_list>
#include <set>
/// separate chaining implementation of hashset
template<typename Key, typename Value, typename hasher = std::hash<Key>>
class hash_table
{
public:
    struct pair
    {
        Key k;
        Value v;
    };
private:
    using bucket = std::forward_list<pair>;
    using bucket_iterator = typename std::forward_list<pair>::iterator;
    using const_bucket_iterator = typename std::forward_list<pair>::const_iterator;

    static constexpr std::size_t INITIAL_BUCKET_CNT = 8;
    static constexpr std::size_t INITIAL_MAX_LOAD_FACTOR = 1;
    static constexpr std::size_t RESIZE_FACTOR = 2;

    std::size_t m_size = 0;
    std::vector<bucket> buckets_array;
    float m_max_load_factor = INITIAL_MAX_LOAD_FACTOR;
public:
    class const_iterator
    {
        friend class hash_table;
        const hash_table& t;
        std::size_t table_index;
        const_bucket_iterator iter;

    public:
        const_iterator(const hash_table& t,
                       std::size_t table_index,
                       const bucket_iterator& iter)
                       : t(t),
                         table_index(table_index),
                         iter(iter)
        {}
        const pair& operator*() {
            return *iter;
        }
        const_bucket_iterator operator->() {
            return iter;
        }
        bool operator == (const const_iterator& other) const {
            return table_index == other.table_index &&
                   iter == other.iter;
        }
        bool operator != (const const_iterator& other) const {
            return ! (*this == other);
        }
        const_iterator& operator++() {
            if(!(t.bucket_count() == table_index + 1 && iter == t.buckets_array[table_index].end())) {
                if(iter == t.buckets_array[table_index].end()){
                    ++table_index;
                    iter = t.buckets_array[table_index].begin();
                }
                else ++iter;
            }
            return *this;
        }
    public:


    };
public:
    void print() const {
        for(const bucket& b : buckets_array) {
            for (const pair& p : b )
                std::cout << p.k << ", " << p.v << "; ";
            std::cout << std::endl;
        }
    }
    hash_table()
        : buckets_array(INITIAL_BUCKET_CNT)
    {}
    hash_table(std::size_t buckets_cnt)
        : buckets_array(buckets_cnt)
    {}

    void rehash(std::size_t new_buckets_cnt) {
        std::size_t min_buckets_cnt = std::ceil(m_size / m_max_load_factor);
        if(new_buckets_cnt < min_buckets_cnt)
            new_buckets_cnt = min_buckets_cnt;
        if(bucket_count() == new_buckets_cnt)
            return;
        std::vector<bucket> new_array(new_buckets_cnt);
        std::swap(buckets_array, new_array);
        m_size = 0;
        for(const bucket& b : new_array)
            for(const pair& p : b)
                insert( { p.k, p.v });
    }
    std::size_t hash_value(const Key& key) const {
        return hasher{}(key) % buckets_array.size();
    }

    std::size_t size() const{
        return m_size;
    }
    std::size_t bucket_count() const {
        return buckets_array.size();
    }
    float load_factor() const {
        std::size_t buckets = buckets_array.size();
        return buckets ? (float)m_size / buckets : 0;
    }
    float max_load_factor() const {
        return m_max_load_factor;
    }
    void max_load_factor(float new_max_load_factor)
    {
        m_max_load_factor = new_max_load_factor;
        std::size_t border_buckets_cnt = std::ceil(m_size / m_max_load_factor);
        if(bucket_count() < border_buckets_cnt)
            rehash(border_buckets_cnt);
    }

    bool contains(const Key& key) const {
        return find(key);
    }

    pair* find(const Key& key) {
        return const_cast<pair*>((const_cast<const hash_table*>(this)) -> find(key));
    }
    const pair* find(const Key& key) const {
        std::size_t index = hash_value(key);
        for(const pair& p : buckets_array[index])
            if(p.k == key)
                return &p;
        return nullptr;
    }

    pair* insert(const pair& p) {
        if(contains(p.k)) return false;
        std::size_t index = hash_value(p.k);
        if(float (m_size + 1) / bucket_count() > max_load_factor())
            rehash(RESIZE_FACTOR * 2);
        buckets_array[index].push_front(p);
        ++m_size;
        return &buckets_array[index].front();
    }
    bool erase(const Key& key) {
        std::size_t index = hash_value(key);
        if(key == buckets_array[index].begin()->k)
            buckets_array[index].pop_front();
        bucket_iterator fast = buckets_array[index].begin();
        bucket_iterator slow = fast++;
        for(; fast != buckets_array[index].end() && fast->k != key; ++fast, ++slow)
            ;
        if(fast != buckets_array[index].end()) {
            buckets_array[index].erase_after(slow);
            --m_size;
            return true;
        }
        return false;

    }
    void clear() {
        buckets_array.clear();
        m_size = 0;
    }

    Value& operator[](const Key& key) {
        return find(key)->v;
    }
    const Value& operator[](const Key& key) const {
        return find(key)->v;
    }
    Value& at(const Key& key) {
        return const_cast<Value&>((const_cast<const hash_table*>(this)) -> at(key));
    }
    const Value& at(const Key& key) const {
        const pair* found = find(key);
        if(found) return found->v;
        throw std::runtime_error("no element with such key in hash table");
    }
    const_iterator begin() const {
        return const_iterator(*this, 0, buckets_array[0].begin());
    }
    const_iterator end() const {
        return const_iterator(*this, m_size - 1, buckets_array[m_size - 1].begin());
    }

    std::multiset<Key> to_multiset() const
    {
        std::multiset<Key> s;
        for(const bucket& b : buckets_array)
            for(const pair& p : b)
                for(std::size_t i = 0; i < p.v; ++i)
                    s.insert(p.k);
        return s;
    }

};

#endif //hash_table_HPP
