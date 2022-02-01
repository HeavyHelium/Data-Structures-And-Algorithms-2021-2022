#ifndef SOLVE_EM_TASKS_LISTS_HPP
#define SOLVE_EM_TASKS_LISTS_HPP

#include <iostream>
#include <cassert>

class list {
    struct node {
        int data = 0;
        node* next = nullptr;
    };
    node* head = nullptr;
    node* tail = nullptr;
    int m_size = 0;
    struct beg_end_len {
        node* beg = nullptr;
        node* end = nullptr;
        int len = 0;
    };
    list(const beg_end_len& t) : head(t.beg), tail(t.end), m_size(t.len) {}
public:
    list() = default;
    list(const std::initializer_list<int>& lst) {
        for(int elem : lst) {
            push_back(elem);
        }
    }
    list(const list& other) {
        beg_end temp = copy_chain(other.head);
        head = temp.beg;
        tail = temp.end;
        m_size = other.m_size;
    }
    // copy-and-swap
    list& operator=(list other) {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(m_size, other.m_size);
        return *this;
    }
    list(list&& other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(m_size, other.m_size);
    }
    ~list() {
        free_chain(head);
    }
    void push_back(int elem) {
        if(!head) {
            assert(!tail);
            head = tail = new node{ elem };
        } else {
            tail = tail->next = new node{ elem };
        }
        ++m_size;
    }
    void push_front(int elem) {
        if(!head) {
            assert(!tail);
            head = tail = new node{ elem };
        } else {
            head = new node{ elem, head };
        }
        ++m_size;
    }
    void pop_front() {
        if(!head) {
            assert(!tail);
            throw std::logic_error("empty list");
        }
        if(head == tail) {
            delete head;
            tail = head = nullptr;
        } else {
            node* to_delete = head;
            head = head->next;
            delete to_delete;
        }
        --m_size;
    }
    const int& front() const {
        return head->data;
    }
    const int& back() const {
        return tail->data;
    }
    template<class Function>
    list max_what_sublist(const Function& predicate) const {
        if(!head) {
            return { };
        }
        node* current_begin = head;
        int current_len = 1;
        node* max_begin = head;
        node* max_end = head;
        int max_len = 1;
        for(node* iter = head; iter->next; iter = iter->next) {
            if(!predicate(iter->data, iter->next->data)) {
                // end of current ascending sublist
                if(current_len > max_len) {
                    max_begin = current_begin;
                    max_end = iter->next;
                    max_len = current_len;
                }
                current_len = 1;
                current_begin = iter->next;
            } else ++current_len;
        }
        if(current_len > max_len) {
            max_begin = current_begin;
            max_len = current_len;
            max_end = nullptr;
        }
        list res{};
        for(node* iter = max_begin; iter != max_end; iter = iter->next) {
            res.push_back(iter->data);
        }
        return res;
    }
    list max_monotonous() const {
        list asnd = max_what_sublist([](int a, int b){ return a > b; });
        list dsnc = max_what_sublist([](int a, int b){ return a < b; });
        return std::move(asnd.m_size > dsnc.m_size ? asnd : dsnc);
    }
    void print() const {
        for(node* iter = head; iter; iter = iter->next) {
            std::cout << iter->data << " ";
        }
        std::cout << std::endl;
    }

    list split() {
        if(!head) {
            return { };
        }
        node* iter = head;
        int m = m_size / 2;
        for(int i = 0; i < m - 1; ++i) {
            iter = iter->next;
        }
        node* new_beg = iter->next;
        tail = iter;
        tail->next = nullptr;
        int other_size = m_size - m;
        m_size = m;
        return { { new_beg, tail, other_size } };
    }
    template<class Function>
    void merge(list& other, const Function& predicate) {
        node dummy;
        node* current = &dummy;
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
        other.head = other.tail = nullptr;
        other.m_size = 0;
        head = dummy.next;
        tail = current;
    }
    template<class Function>
    void merge_sort(const Function& predicate) {
        if(m_size < 2) {
            return;
        }
        list half2(split());// copy elision
        merge_sort(predicate);
        half2.merge_sort(predicate);
        merge(half2, predicate);
    }
    void solution() {
        list odd(steal_select([](int a){ return a % 2 == 0; }));
        odd.print();
        merge_sort([](int a, int b){ return a < b; });
        odd.merge_sort([](int a, int b){ return a > b; });
        print();
        concat(odd);
    }
    void concat(list& other) {
        if(!head) {
            *this = std::move(other);
            return;
        }
        if(!other.head) {
            return;
        }
        tail->next = other.head;
        tail = other.tail;
        m_size += other.m_size;
        other.tail = other.head = nullptr;
        other.m_size = 0;
    }
    void to_set() {
        if(m_size < 2) {
            return;
        }
        node dummy;
        node* current = & dummy;
        node* iter = head;
        while(iter->next) {
            if(iter->data == iter->next->data) {
                node* to_delete = iter;
                iter = iter->next;
                delete to_delete;
                --m_size;
            } else {
                current = current->next = iter;
                iter = iter->next;
            }
        }
        current = current->next = tail;
        head = dummy.next;
    }
private:
    struct beg_end {
        node* beg = nullptr;
        node* end = nullptr;
    };
    template<class Function>
    beg_end_len steal_select(const Function& predicate) {
        if(!head) {
            return {};
        }
        node dummy1;
        node* current1 = &dummy1;
        node dummy2;
        int stolen_size = 0;
        node* current2 = &dummy2;
        for(node* iter = head; iter; iter = iter->next) {
            if(predicate(iter->data)) {
                current1 = current1->next = iter;
                --m_size;
            } else {
                current2 = current2->next = iter;
                ++stolen_size;
            }
        }
        current1->next = nullptr;
        current2->next = nullptr;
        head = dummy1.next;
        tail = current1;
        return { dummy2.next, current2, stolen_size };
    }
    static beg_end copy_chain(const node* other) {
        if(!other) {
            return {};
        }
        node* beg = new node{ other->data };
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
            node* t_del = root;
            root = root->next;
            delete t_del;
        }
    }

};

int main() {
    list lst{ 2, 1, 1, 2, 5, 7, 18, 16, 16, 3 };
    lst.merge_sort([](int a, int b) { return a < b; });
    lst.print();
    lst.to_set();
    lst.print();
    lst.solution();
    lst.print();
    list lst2 { 1, 3 };
    list lst3 { 2 };
    return 0;
}

#endif //SOLVE_EM_TASKS_LISTS_HPP
