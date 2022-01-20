//#include "task1_e1.h"
//#include "task2_e1.h"
//#include "task1_e2.h"
//#include "task2_e2.h"
#include <iostream>
#include <cstddef>
#include <cassert>
#include <initializer_list>

class list {
    struct Node{
        int val = 0;
        Node* next = nullptr;
    };
    Node* first = nullptr;
    Node* last = nullptr;
    std::size_t m_size = 0;
public:
    class iterator {
        Node* ptr;
        friend class list;
    public:
        iterator(Node* ptr) : ptr(ptr) {}
        iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        iterator operator++(int) {
            iterator copy(*this);
            ++*this;
            return copy;
        }
        bool operator==(const iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
        int& operator*() {
            return ptr->val;
        }
        int* operator->() {
            return &(ptr->val);
        }
    };
    class const_iterator {
        Node* ptr;
        friend class list;
    public:
        const_iterator(Node* ptr) : ptr(ptr) {}
        const_iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator copy(*this);
            ++*this;
            return copy;
        }
        bool operator==(const const_iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }
        int& operator*() {
            return ptr->val;
        }
        int* operator->() {
            return &(ptr->val);
        }
    };
    list() = default;
    list(list&& other) {
        std::swap(first, other.first);
        std::swap(last, other.last);
        std::swap(m_size, other.m_size);
    }
    list(const list& other)
    {
        copy_chain(other.first);
        m_size = other.m_size;
    }
    list& operator=(list other) {
        std::swap(first, other.first);
        std::swap(last, other.last);
        std::swap(m_size, other.m_size);
        return *this;
    }
    list(std::initializer_list<int> lst) {
        for(int i : lst) {
            push_back(i);
        }
    }
    void push_back(int val) {
        if(!first) {
            assert(!last);
            first = last = new Node{ val };
        }
        else {
            assert(last);
            last = last->next = new Node { val };
        }
        ++m_size;
    }
    void push_front(int val) {
        if(!first) {
            assert(!last);
            first = last = new Node{ val };
        }
        else {
            assert(last);
            first = new Node{ val, first };
        }
        ++m_size;
    }
    void pop_front() {
        assert(m_size);
        if(first == last) {
            delete first;
            first = last = nullptr;
        }
        else {
            Node* to_delete = first;
            first = first->next;
            delete to_delete;
        }
        --m_size;
    }
    std::size_t size() const {
        return m_size;
    }
    bool empty() const {
        return !m_size;
    }
    iterator begin() {
        return { first };
    }
    iterator end() {
        return { nullptr };
    }
    const_iterator begin() const {
        return { first };
    }
    const_iterator end() const {
        return { nullptr };
    }
    list filter() {
        list result;
    }
private:
    void copy_chain(const Node* other) {
        assert(first == last && last == nullptr);
        if(!other) {
            return;
        }
        Node* res = new Node{ other->val };
        Node* iter = res;
        try {
            while(other = other->next) {
                iter = iter->next = new Node{ other->val };
            }
        }
        catch(...) {
            free_chain(res);
            throw;
        }
        first = res;
        last = iter;
    }
    void free_chain(Node* chain) {
        if(!chain) {
            return;
        }
        while(chain) {
            Node* to_delete = chain;
            chain = chain->next;
            delete to_delete;
        }
    }
};

int main() {
    list l1{ 1, 2, 3, 4, 5 };

    for( int i : l1) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;
    l1.pop_front();
    for( int i : l1) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;
    char ch;
    std::cin.get(ch);
    return 0;
}