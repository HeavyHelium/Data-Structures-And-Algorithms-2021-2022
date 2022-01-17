#ifndef TASKS_LIST_H
#define TASKS_LIST_H
#include <cassert>
#include <iostream>
#include <initializer_list>


class list {
    struct Node {
        int val;
        Node* next = nullptr;
    };
    Node* first = nullptr;
    Node* last = nullptr;
public:
    class iterator {
        Node* ptr = nullptr;
        friend class list;
    public:
        iterator(Node* ptr)
            : ptr(ptr)
        {}
        iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        iterator operator++(int) {
            iterator copy = *this;
            ++(*this);
            return copy;
        }
        int& operator*() {
            return ptr->val;
        }
        int* operator->() {
            return &ptr->val;
        }
        bool operator==(const iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };
    class const_iterator {
        const Node* ptr = nullptr;
        friend class list;
    public:
        const_iterator(const Node* ptr)
                : ptr(ptr)
        {}
        const_iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator copy = *this;
            ++(*this);
            return copy;
        }
        const int& operator*() {
            return ptr->val;
        }
        const int* operator->() {
            return &ptr->val;
        }
        bool operator==(const const_iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }
    };
    list() = default;
    void push_back(int val) {
        if(!first) {
            assert(!last);
            last = first = new Node{ val };
        }
        else {
            last = last->next = new Node{val};
        }
        assert(!empty());
    }
    void push_front(int val) {
        if(!first){
            last = first = new Node{ val };
        }
        else first = new Node{ val, first };
    }
    list(std::initializer_list<int> lst) {
        for(int elem : lst) {
            push_back(elem);
        }
    }
    list(const list& other) {
        clone_chain(other.first);
    }
    list& operator=(const list& other) {
        if(this != &other) {
            std::cout << "ggghh\n";
            free_chain(first);
            last = first = nullptr;
            clone_chain(other.first);
        }
        return *this;
    }
    void pop_front() {
        assert(!empty());
        if(first == last) {
            delete first;
            last = first = nullptr;
        }
        Node* to_delete = first;
        first = first->next;
        delete first;
    }
    bool empty() const {
        return !first;
    }
    ~list() {
        free_chain(first);
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
    list(const_iterator b, const_iterator e) {
        for(;b != e; ++b) {
            push_back(*b);
        }
    }
    list(iterator b, iterator e) {
        for(; b != e; ++b) {
            push_back(*b);
        }
    }
private:
    void clone_chain(const Node* other) {
        assert(!first);
        if(!other) {
            return;
        }
        Node* new_chain = new Node{ other->val };
        Node* beg = new_chain;
        while(other = other->next) {
            new_chain = new_chain->next = new Node{ other->val };
        }
        first = beg;
        last = new_chain;
    }
    static void free_chain(Node* chain) {
        while(chain){
            Node* to_delete = chain;
            chain = chain->next;
            delete to_delete;
        }
    }
};



#endif //TASKS_LIST_H
