#include <iostream>
#include <initializer_list>

struct list {
    struct node {
        int key;
        node* next = nullptr;
    };
    list(std::initializer_list<int> lst) {
        for(auto elem : lst) {
            push_back(elem);
        }
    }
    list(const list& other) {
        lst_info cpy = copy_chain(other.begin);
        begin = cpy.begin;
        end = cpy.end;
        m_size = cpy.size;
    }
    // copy-and-swap idiom
    list& operator=(list other) {
        std::swap(begin, other.begin);
        std::swap(end, other.end);
        std::swap(m_size, other.m_size);
        return *this;
    }
    list(list&& other) {
        std::swap(begin, other.begin);
        std::swap(end, other.end);
        std::swap(m_size, other.m_size);
    }

    void push_back(int key) {
        if(!begin) {
            begin = end = new node{key};
        } else {
            end = end->next = new node{ key };
        }
        ++m_size;
    }

    void rotate(int k) {
        if(!k) {
            return;
        }
        k = k % m_size;
        node* iter = begin;
        node* slow = iter;
        while(k--) {
            slow = iter;
            iter = iter->next;
        }
        end->next = begin;
        begin = iter;
        end = slow;
        end->next = nullptr;
    }

    int size() const {
        return m_size;
    }

    void print() {
        node* iter = begin;
        while(iter) {
            std::cout << iter->key << " ";
            iter = iter->next;
        }
        std::cout << std::endl;
    }

    void clean() {
        node* iter = begin;
        node* to_del;
        while(iter) {
            to_del = iter;
            iter = iter->next;
            delete to_del;
        }
        m_size = 0;
    }

    ~list() {
        clean();
    }

private:
    struct lst_info {
        node* begin{ nullptr };
        node* end{ nullptr };
        int size{ 0 };
    };
    static lst_info copy_chain(const node* other_begin) {
        if(!other_begin) {
            return {  };
        }
        const node* iter = other_begin;
        node* current = new node{ other_begin->key };
        node* new_begin = current;
        int size{ 1 };
        while(iter = iter->next) {
            current = current->next = new node{ iter->key };
            ++size;
        }
        return { new_begin, current, size };
    }
    node* begin = nullptr;
    node* end = nullptr;
    int m_size{ 0 };
};

int main() {
    list lst{ 1, 2, 3, 4, 5, 6 };
    lst.print();
    lst.rotate(2);
    lst.print();
    lst.rotate(3);
    lst.print();
    lst.rotate(7);
    lst.print();
    return 0;
}

