#include <iostream>
#include <cassert>
#include <stdexcept>
#include <initializer_list>
#include <algorithm>

class list {
    struct node {
        int data = 0;
        node* next = nullptr;
    };
    node* head = nullptr;
    node* tail = nullptr;
    int m_size = 0;
    class iterator {
        friend class list;
        node* ptr;
    public:
        iterator(node* ptr) : ptr(ptr) { }
        iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        iterator operator++(int) {
            iterator copy = *this;
            ++*this;
            return copy;
        }
        int& operator*() {
            return ptr->data;
        }
        int* operator->() {
            return &ptr->data;
        }
        bool operator!= (const iterator& other) const {
            return ptr != other.ptr;
        }
    };
    class const_iterator {
        friend class list;
        const node* ptr;
    public:
        const_iterator(const node* ptr) : ptr(ptr) { }
        const_iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator copy = *this;
            ++*this;
            return copy;
        }
        const int& operator*() {
            return ptr->data;
        }
        const int* operator->() {
            return &ptr->data;
        }
        bool operator!= (const const_iterator& other) const {
            return ptr != other.ptr;
        }
    };
    struct beg_end_size{
        node* beg = nullptr;
        node* end = nullptr;
        int len = 0;
    };
public:
    list() = default;
    list(list&& other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(m_size, other.m_size);
    }
    list(const list& other) {
        beg_end e = copy_chain(other.head);
        head = e.beg;
        tail = e.end;
        m_size = other.m_size;
    }
    list(const std::initializer_list<int>& lst) {
        for(int elem : lst) {
            push_back(elem);
        }
    }
    list(const beg_end_size& tuple)
    : head(tuple.beg),
      tail(tuple.end),
      m_size(tuple.len) {
    }
    // copy-and-swap
    list& operator=(list other) {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(m_size, other.m_size);
        return *this;
    }
    ~list() {
        free_chain(head);
    }
    void push_back(const int& elem) {
        if(!head) {
            assert(!tail);
            head = tail = new node { elem };
        } else {
            tail = tail->next = new node { elem };
        }
        ++m_size;
    }
    void push_front(const int& elem) {
        if(!head) {
            assert(!tail);
            head = tail = new node { elem };
        } else {
            head = new node { elem, head };
        }
    }
    void pop_front() {
        if(!head) {
            assert(!tail);
            throw  std::logic_error("empty list");
        }
        if(head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            node* to_del = head;
            head = head->next;
            delete to_del;
        }
    }

    iterator begin() {
        return { head };
    }
    iterator end() {
        return { nullptr };
    }

    const_iterator begin() const {
        return { head };
    }
    const_iterator end() const {
        return { nullptr };
    }

    bool ascending() const {
        for(const node* iter = head; iter && iter->next; iter = iter->next) {
            if(iter->data > iter->next->data) {
                return false;
            }
        }
        return true;
    }

    template<class F>
    void merge(list& other, F predicate) {
        node dummy;
        node* current = & dummy;
        node* iter1 = head;
        node* iter2 = other.head;
        while(iter1 && iter2) {
            if(predicate(iter1->data, iter2->data)) {
                current = current->next = iter1;
                iter1 = iter1->next;
            } else {
                current = current->next = iter2;
                iter2 = iter2->next;
            }
        }
        while(iter1) {
            current = current->next = iter1;
            iter1 = iter1->next;
        }
        while(iter2) {
            current = current->next = iter2;
            iter2 = iter2->next;
        }
        m_size += other.m_size;
        head = dummy.next;
        tail = current;
        other.head = other.tail = nullptr;
        other.m_size = 0;
    }

    list split() {
        if(m_size < 2) return {};
        int middle = m_size / 2;
        node* end_node = head;
        for(int i = 0; i < middle - 1; ++i) {
            end_node = end_node->next;
        }
        list snd_half{ { end_node->next, tail, m_size - middle } };
        tail = end_node;
        tail->next = nullptr;
        m_size = middle;
        return snd_half;// copy elision will occur
    }

    template<class F>
    void merge_sort(const F& predicate) {
        if(m_size < 2) {
            return;
        }
        list second_half = split();
        merge_sort(predicate);
        second_half.merge_sort(predicate);
        merge(second_half, predicate);
    }

    void reverse() {
        if(!head) {
            assert(!tail);
            return;
        }
        node* prev = nullptr;
        node* current = head;
        node* next = nullptr;
        while(current) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        tail = head;
        head = prev;
    }

    void to_set() {
       if(!head || !head->next) return;
       node dummy;
       node* current = &dummy;
       node* fast = head;
       while(fast && fast->next) {
           if(fast->data != fast->next->data) {
               current = current->next = fast;
               fast = fast->next;
           } else {
               node* to_delete = fast;
               fast = fast->next;
               delete to_delete;
               --m_size;
           }
       }
       current->next = fast;
       head = dummy.next;
       tail = current;
    }

    template<typename F>
    void filter(const F& predicate) {
        node dummy;
        node* current = &dummy;
        for(node* iter = head; iter; ) {
            if(predicate(iter->data)) {
                current = current->next = iter;
                iter = iter->next;
            } else {
                node* to_delete = iter;
                iter = iter->next;
                delete to_delete;
                --m_size;
            }
        }
        current->next = nullptr;
        head = dummy.next;
        tail = current;
    }

    template<typename F>
    list steal(const F& predicate) {
        return { steal_chain(predicate) };
    }
private:
    struct beg_end {
        node* beg = nullptr;
        node* end = nullptr;
    };
    static beg_end copy_chain(const node* other) {
        if(!other) {
            return { };
        }
        node* beg = new node { other->data };
        node* current = beg;
        other = other->next;
        while(other) {
            current = current->next = new node{ other->data };
            other = other->next;
        }
        return { beg, current };
    }
    static void free_chain(node* root) {
        while(root) {
            node* to_del = root;
            root = root->next;
            delete to_del;
        }
    }
    template<typename F>
    beg_end_size steal_chain(const F& predicate) {
        node dummy1;
        node dummy2;
        node* current1 = &dummy1;
        node* current2 = &dummy2;
        int size2 = 0;
        for(node* iter = head; iter; iter = iter->next) {
            if(!predicate(iter->data)) {
                current1 = current1->next = iter;
            } else {
                current2 = current2->next = iter;
                ++size2;
                --m_size;
            }
        }
        current1->next = nullptr;
        current2->next = nullptr;
        head = dummy1.next;
        tail = current1;
        return { dummy2.next, current2, size2 };
    }
};

int main() {
    list lst { 1, 3};
    list lst1 { 2, 4, 6, 6 };
    list lst2{ 5, 4, 2, 1, 3, 6 };
    //auto f = [](int a){ return a % 2 != 0; };
    //list lst2(lst.steal(f));
    //lst.to_set();
    auto predicate = [](int a, int b) { return a < b; };
    lst2.merge_sort(predicate);
    lst2.reverse();
    //lst1.merge(lst, predicate);
    //list c = lst.split();
    for(int elem : lst2) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    list snd = lst1.split();
    for(int elem : lst1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    for(int elem : snd) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    std::cout << std::boolalpha << lst.ascending() << std::endl;

    /*
    list h = lst2.split();
    for(int elem : lst2) {
        std::cout << elem << " ";
    }
    std::cout << "h1\n";
    list h1 = lst2.split();
    for(int elem : h1) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;*/
    return 0;
}
