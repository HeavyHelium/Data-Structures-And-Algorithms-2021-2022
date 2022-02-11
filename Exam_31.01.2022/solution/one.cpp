#include <iostream>
#include <typeinfo>
#include <cassert>
#include <stdexcept>

static constexpr int drink_cnt = 8;
enum class drink_type {
    beer,
    vodka,
    whiskey,
    tequila,
    coke,
    rom,
    gin,
    mint
};

struct slot_data {
    bool drinks_available[drink_cnt] = { 0, };
    bool taken = false;
};

struct bar_slot {
    slot_data data;
    bar_slot* next = nullptr;
};
struct client_data {
    bool drinks_wanted[drink_cnt] = { 0,  };
};

struct client {
    client_data data;
    client* prev = nullptr;
    client* next = nullptr;
};

class forward_list {
    bar_slot* head = nullptr;
    bar_slot* tail = nullptr;
    std::size_t m_size = 0;
public:
    forward_list() = default;
    forward_list(forward_list& other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(m_size, other.m_size);
    }
    forward_list(const forward_list& other) {
        beg_end chain{ copy_chain(other.head) };
        head = chain.beg;
        tail = chain.end;
        m_size = other.m_size;
    }
    forward_list& operator=(forward_list other) {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(m_size, other.m_size);
        return *this;
    }
    ~forward_list() {
        free_chain(head);
    }
    void push_back(const slot_data& elem) {
        if(!head) {
            assert(!tail);
            head = tail = new bar_slot{ elem };
        } else {
            assert(tail);
            tail = tail->next = new bar_slot{ elem };
        }
        ++m_size;
    }
    void push_front(const slot_data& elem) {
        if(!head) {
            assert(!tail);
            head = tail = new bar_slot{ elem };
        } else {
            head = new bar_slot{ elem, head };
        }
        ++m_size;
    }
    void pop_front() {
        if(!head) {
            throw std::logic_error{ "cannot remove elements from an empty list" };
        }
        if(head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            bar_slot* to_delete = head;
            head = head->next;
            delete to_delete;
        }
        --m_size;
    }
private:
    struct beg_end {
        bar_slot* beg = nullptr;
        bar_slot* end = nullptr;
    };
    static beg_end copy_chain(const bar_slot* other) {
        if(!other) {
            return {};
        }
        bar_slot* b = new bar_slot{ other->data };
        bar_slot* current = b;
        other = other->next;
        while(other) {
            current = current->next = new bar_slot{ other->data };
            other = other->next;
        }
        return { b, current };
    }
    static void free_chain(bar_slot* h) {
        while(h) {
            bar_slot* to_delete = h;
            h = h->next;
            delete to_delete;
        }
    }
};

class list {
    client* head = nullptr;
    client* tail = nullptr;
    std::size_t m_size = 0;
public:
    list() = default;
    void push_back(const client_data& d) {
        if(!head) {
            assert(!tail);
            head = tail = new client{ d };
        } else {
            tail = tail->next = new client{ d, tail };
        }
        ++m_size;
    }
    void push_front(const client_data& d) {
        if(!head) {
            head = tail = new client{ d };
        } else {
            head = new client{ d, nullptr, head };
        }
    }
};

int main() try {
    std::cout << "task1" << std::endl;
    char ch;
    std::cin.get(ch);
    return 0;
}
catch(const std::exception& e) {
    std::cerr << "something went terribly wrong" <<
                  typeid(e).name() << ": " <<
                  e.what() << std::endl;
    return -1;
}

