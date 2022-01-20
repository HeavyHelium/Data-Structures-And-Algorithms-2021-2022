//#include "task1_e1.h"
//#include "task2_e1.h"
//#include "task1_e2.h"
//#include "task2_e2.h"
#include <iostream>
#include <cstddef>
#include <cassert>
#include <initializer_list>
#include <algorithm>

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
    void reverse() {
        Node* prev = nullptr;
        Node* current = first;
        Node* next = nullptr;
        while(current) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        last = first;
        first = prev;
    }
    void to_set() {
        Node* slow, * fast, * detach;
        slow = first;
        while(slow && slow->next) {
            fast = slow;
            while(fast && fast->next) {
                detach = fast->next;
                if(detach->val == fast->val) {
                    fast->next = detach->next;
                    delete detach;// и чиста работа :Д
                    --m_size;
                }
                else fast = fast->next;
            }
            slow = slow->next;
        }
        last = slow;
    }
    template<typename T>
    void filter(const T& predicate) {
        Node dummy;
        Node* current = &dummy;
        Node* to_delete = nullptr;
        for(Node* iter = first; iter; iter = iter->next) {
            delete to_delete;
            current->next = iter;
            if(predicate(iter->val)) {
                current = current->next;
                to_delete = nullptr;
            }
            else {
                to_delete = iter;
                --m_size;
            }
        }
        delete to_delete;// because the last element might not satisfy the predicate
        // even if it does, life is good, because we're deleting a nullptr
        if(m_size) {
            current->next = nullptr;
            first = dummy.next;
            last = current;
        }
        else {
            first = last = nullptr;
        }
    }
    template<typename T>
    list steal(const T& predicate) {
        if(empty()) {
            return {};
        }
        Node dummy1;
        Node dummy2;
        int initial_size = m_size;
        Node* current = &dummy1;
        Node* current_stolen = &dummy2;
        Node* to_steal = nullptr;
        for(Node* iter = first; iter; iter = iter->next) {
            if(to_steal)
                current_stolen = current_stolen->next = to_steal;
            current->next = iter;
            if(!predicate(iter->val)) {
                current = current->next;
                to_steal = nullptr;
            }
            else {
                to_steal = iter;
                --m_size;
            }
        }
        if(to_steal)
            current_stolen = current_stolen->next = to_steal;
        if(!m_size) {
            first = last = nullptr;
            current_stolen->next = nullptr;
            return list(dummy2.next, current_stolen, m_size);
        } else if(m_size == initial_size) {
            return {};
        } else {
            current_stolen->next = nullptr;
            current->next = nullptr;
            first = dummy1.next;
            last = current;
            return list(dummy2.next, current_stolen, initial_size - m_size);
        }
    }
    void do_your_magic() {
        list odd = steal([](int a){ return a % 2 != 0; });
        sort([](int a, int b) { return a < b; });
        odd.sort([](int a, int b) { return a > b; });
        append(odd);
        to_set();
    }
    static void split(Node* head,
                      Node*& left,
                      Node*& right,
                      std::size_t list_len) {
        Node* iter = head;
        left = iter;
        std::size_t middle = list_len / 2 + list_len %  2;
        for(std::size_t i = 0; i < middle - 1; ++i) {
            iter = iter->next;
        }
        right = iter->next;
        iter->next = nullptr;
    }
    template<typename T>
    static Node* merge(Node* left, Node* right, const T& predicate) {
        Node merged;
        Node* current = &merged;
        while(left && right) {
            if(predicate(left->val, right->val)) {
                current = current->next = left;
                left = left->next;
            }
            else {
                current = current->next = right;
                right = right->next;
            }
        }
        current->next = left ? left : right;
        return merged.next;
    }
    template<typename T>
    static Node* merge_sort(Node* list, std::size_t size, const T& predicate) {
        if(list->next == nullptr) {
            return list;
        }
        Node* left = nullptr, * right = nullptr;
        split(list, left, right, size);
        Node* res  = merge(merge_sort(left, size / 2 + size % 2, predicate),
                     merge_sort(right, size / 2, predicate),
                     predicate);
        return res;
    }
    template<class T>
    void sort(const T& predicate) {
       first = merge_sort(first, m_size, predicate);
       fix_last();
    }
    void fix_last() {
        if(!first) {
            last = first;
            return;
        }
        Node* iter;
        for(iter = first; iter->next; iter = iter->next)
            ;
        last = iter;
    }

    template<typename T>
    void merge(list& other, T predicate) {
        Node dummy;
        Node* current = &dummy;
        while(first && other.first) {
            if(predicate(first->val, other.first->val)) {
                current = current->next = other.first;
                other.first = other.first->next;
            } else {
                current = current->next = first;
                first = first->next;
            }
        }
        while(first) {
            current = current->next = first;
            first = first->next;
        }
        while(other.first) {
            current = current->next = other.first;
            other.first = other.first->next;
        }
        first = dummy.next;
        last = current;
        m_size += other.m_size;
        other.first = other.last = nullptr;
        other.m_size = 0;
    }
    void append(list& other) {
        if(!first) {
            *this = std::move(other);
        } else {
            last->next = other.first;
            last = other.last;
            m_size += other.m_size;
            other.first = other.last = nullptr;
            other.m_size = 0;
        }
    }
private:
    list(Node* first, Node* last, std::size_t count)
        : first(first),
          last(last),
          m_size(count)
    {}
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

int main() try {
    list l1{ 7, 1, 4, 7, 3, 2, 5, 8, 1, 3, 6, 8, 8 };
    std::cout << "before transformation:\n";
    for(int i : l1) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    l1.do_your_magic();
    std::cout << "after transformation:\n";
    for( int i : l1) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "\n...Goodbye!\n...press any key to close the program\n";
    char ch;
    std::cin.get(ch);
    return 0;
}
catch(const std::exception& e) {
    std::cerr << "an unexpected error occurred with message: "
              << e.what() << '\n';
    return -1;
}