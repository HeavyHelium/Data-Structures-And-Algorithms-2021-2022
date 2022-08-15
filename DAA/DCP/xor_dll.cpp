#include <iostream>
#include <cassert>
#include <cstdint>

/*
Explanation:

-> we can traverse the list in both forward and reverse directions
-> while traversing the list we need to remember the address
   of the previously accessed node in order to calculate
   the next node's address
        -> exmp : we're at node C and have the address of B
           address(B) ^ npx(C) = address(D)

*/

/// TODO : maybe write an iterator(should be initialized with prev and current)


///@brief an XOR doubly linked list implementation
/// has the perks of bidirectional iteration(well, I have to implement iterators, but you get the point)
/// and simultaneously the perks of storing only one pointer in node
/// by virtue of XOR :)
template<typename T>
class x_linked_list {
    struct node {
        T data;
        node* npx; // xor of next and previous
    };
    static node* XOR(node* x, node* y) {
        return reinterpret_cast<node*>(
               reinterpret_cast<uintptr_t>(x) ^
               reinterpret_cast<uintptr_t>(y));
    }
    static const node* XOR(const node* x, const node* y) {
        return reinterpret_cast<node*>(
               reinterpret_cast<uintptr_t>(x) ^
               reinterpret_cast<uintptr_t>(y));
    }
    int m_size{ 0 };
    node* first{ nullptr };
    node* last{ nullptr };
public:
    x_linked_list() = default;
    x_linked_list(const x_linked_list& other) {
        node const* current = other.first;
        const node* prev = nullptr;
        const node* next;
        try {
            while(current) {
                push_back(current->data);
                next = XOR(prev, current->npx);
                prev = current;
                current = next;
            }
        } catch(...) {
            this->~x_linked_list();
            throw;
        }
    }
    x_linked_list& operator=(x_linked_list other) {
        std::swap(other.first, first);
        std::swap(other.last, last);
        std::swap(other.m_size, last.m_size);
        return *this;
    }
    x_linked_list(x_linked_list&& other) {
        std::swap(other.first, first);
        std::swap(other.last, last);
        std::swap(other.m_size, last.m_size);
    }
    ~x_linked_list() {
        node* current = first;
        node* prev = nullptr;
        node* next;
        while(current) {
            next = XOR(prev, current->npx);
            prev = current;
            delete current;
            current = next;
        }
    }
    void push_back(const T& data) {
        if(!first) {
            assert(!last && "pointer error");
            first = last = new node{ data, last };
        } else {
            node* new_node = new node{ data, last };
            last->npx = XOR(last->npx, new_node);
            last = new_node;
        }
        ++m_size;
    }
    void push_front(const T& data) {
        if(!first) {
            assert(!last && "pointer error");
            first = last = new node{ data, first };
        } else {
            node* new_node = new node{ data, first };
            first->npx = XOR(new_node, first->npx);
            first = new_node;
        }
        ++m_size;
    }
    void pop_front() {
        if(!first) {
            assert(!last && "pointer error");
            throw std::runtime_error("popping from an empty list is not allowed");
        } else {
            node* to_del = first;
            first = first->npx;
            delete to_del;
            --m_size;
        }
    }
    void print() const {
        node* current = first;
        node* prev = nullptr;
        node* next;
        while(current) {
            std::cout << current->data << " ";
            next = XOR(prev, current->npx);
            prev = current;
            current = next;
        }
        std::cout << std::endl;
    }
    int size() const {
        return m_size;
    }
};

int main() {
    std::cout << 42 << std::endl;
    x_linked_list<int> lst;
    lst.push_back(14);
    lst.push_back(42);
    lst.print();
    x_linked_list<int> lst1(lst);
    lst1.print();
    //std::cout << ((5 ^ 6) ^ 5) << std::endl;
    // xoring with zero is ok, 0 is the neutral element ->
    // we get the same number
    return 0;
}
