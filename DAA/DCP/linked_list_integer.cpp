#include <iostream>
#include <initializer_list>
#include <utility>


struct linked_int {

    struct node {
        int data;
        node* next{ nullptr };
    };

    struct iterator {
        iterator(node* ptr) : ptr(ptr) {
        }
        iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        iterator operator++(int) {
            iterator copy(*this);
            ++*this;
            return std::move(copy);
        }
        bool operator!=(const iterator& other) const {
            return ptr != other.ptr;
        }
        int& operator*() {
            return ptr->data;
        }
        const int& operator*() const {
            return ptr->data;
        }
        const int* operator->() const {
            return &(ptr->data);
        }
        int* operator->() {
            return &(ptr->data);
        }
        private:
        node* ptr = nullptr;
    };

    struct const_iterator {
        const_iterator(const node* ptr) : ptr(ptr) {
        }
        const_iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator copy(*this);
            ++*this;
            return std::move(copy);
        }
        bool operator!=(const const_iterator& other) const {
            return ptr != other.ptr;
        }
        const int& operator*() const {
            return ptr->data;
        }
        const int* operator->() const {
            return &(ptr->data);
        }
    private:
        const node* ptr{ nullptr };
    };

    linked_int(std::initializer_list<int> lst) {
        for(int i : lst) {
            push_back(i);
        }
    }

    linked_int(int a) {
        int d;
        while(a) {
            d = a % 10;
            push_back(d);
            a /= 10;
        }
    }

    linked_int operator+(const linked_int& other) const {
        return (int)*this + (int)(other);
    }

    void push_back(int data) {
        if(empty()) {
            begin_ptr = end_ptr = new node{ data };
        } else {
            end_ptr = end_ptr->next = new node{ data };
        }
    }

    bool empty() const {
        return !begin_ptr;
    }

    operator int() const {
        int res{ 0 };
        int base{ 1 };
        for(auto elem : *this) {
            res += base * elem;
            base *= 10;
        }
        return res;
    }

    iterator begin() {
        return begin_ptr;
    }
    iterator end() {
        return nullptr;
    }

    const_iterator begin() const {
        return begin_ptr;
    }
    const_iterator end() const {
        return nullptr;
    }

    ~linked_int() {
        node* iter{ begin_ptr };
        node* to_del{ iter };
        while(iter) {
            iter = iter->next;
            delete to_del;
            to_del = iter;
        }
    }

    private:
    node* begin_ptr{ nullptr };
    node* end_ptr{ nullptr };

};


int main() { 
    linked_int lst0{ 1, 2, 3, 4, 5 };
    for(auto iter = lst0.begin(); iter != lst0.end(); ++iter) {
        std::cout << *iter << " ";
    }
    std::cout << lst0 << std::endl;

    linked_int lst1(25);
    std::cout << lst1 << std::endl;

    std::cout << lst0 + lst1 << std::endl;

    return 0;
}
