// 
// СУ "Св. Климент Охридски"
// Факултет по математика и информатика
// 
// Курсове Структури от данни и програмиране
//         Структури от данни
// Зимен семестър 2021/22 г.
// 
// Практически изпит
// 
// Име: Диана Венциславова Маркова
// ФН: 82140
// Специалност: Компютърни науки
// Курс: 2(втори)
// Административна група: 1(първа)
// Ден, в който се явявате на изпита: 31.01.2022(понеделник)
// Начален час на изпита: 09:00
// Кой компилатор използвате: g++
//
//#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cassert>
//#include <stdexcept>
//#include <fstream>
//#include <cstddef>
//#include <algorithm>
//#include <utility>
//#include <vector>
//#include <string>

enum drink_type {
    beer,
    vodka,
    whiskey,
    tequila,
    cola,
    rom,
    jin,
    mint,
};

struct BarSlot {
    bool drinks_array[8] = { 0, };
    BarSlot* next = nullptr;
    bool taken = false;
    BarSlot(const bool* arr, BarSlot* next = nullptr) : next(next) {
        for(int i = 0; i < 8; ++i) {
            drinks_array[i] = arr[i];
        }
    }
    friend std::ostream& operator<<(std::ostream& os, const BarSlot& b) {
        os << "drinks: ";
        for(bool i : b.drinks_array) {
            os << i << ", ";
        }
        os << std::endl;

    }
};
class forward_list {
    BarSlot* head = nullptr;
    BarSlot* tail = nullptr;
    int m_size = 0;
public:
    forward_list() = default;
    forward_list(const forward_list& other) {
        beg_end t = copy_chain(other.head);
        head = t.beg;
        tail = t.end;
        m_size = other.m_size;
    }
    forward_list& operator=(forward_list other) {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(m_size, other.m_size);
        return *this;
    }
    forward_list(forward_list&& other) {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(m_size, other.m_size);
    }
    void push_back(const bool* d) {
        if(!head) {
            assert(!tail);
            head = tail = new BarSlot(d);
        } else {
            tail = tail->next = new BarSlot(d);
        }
        ++m_size;
    }
    void pop_front() {
        if(!head) {
            assert(!tail);
            throw std::logic_error("list is empty");
        }
        if(head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            BarSlot* to_delete = head;
            head = head->next;
            delete to_delete;
        }
        --m_size;
    }
    void push_front(const bool* b) {
        if(!head) {
            head = tail = new BarSlot(b);
        } else {
            head = new BarSlot(b, head);
        }
        ++m_size;
    }
    ~forward_list() {
        free(head);
    }
    void print() {
        for(BarSlot* iter = head; iter; iter = iter->next) {
            std::cout << *iter << std::endl;
        }
    }
private:
    struct beg_end {
        BarSlot* beg = nullptr;
        BarSlot* end = nullptr;
        //int len = 0;
    };
    static void free(BarSlot* head) {
        while(head) {
            BarSlot* to_del = head;
            head = head->next;
            delete to_del;
        }
    }
    static beg_end copy_chain(const BarSlot* head) {
        if(!head) return { };
        BarSlot* beg = new BarSlot(head->drinks_array);
        BarSlot* current = beg;
        head = head->next;
        while(head) {
            current = current->next = new BarSlot(head->drinks_array);
            head = head->next;
        }
        return { beg, current };
    }
};

struct Client {
    bool drinks[8] = { 0, };
    Client* prev = nullptr;
    Client* next = nullptr;
    Client(const bool* d, Client* prev = nullptr, Client* next = nullptr)
        : prev(prev), next(next) {
        for(int i = 0; i < 8; ++i) {
            drinks[i] = d[i];
        }
    }
};

class dl_list {
    Client* head = nullptr;
    Client* tail = nullptr;
    int m_size = 0;
public:
    dl_list() = default;
    dl_list(const dl_list& other) = delete;
    dl_list& operator=(const dl_list& other) = delete;
    void push_back(const bool* d) {
        if(!head) {
            head = tail = new Client(d);
        } else {
            tail = tail->next = new Client(d, tail);
        }
        ++m_size;
    }
    ~dl_list(){
        free_chain(head);
    }
private:
    static void free_chain(Client* head) {
        while(head) {
          Client* to_del = head;
          head = head->next;
          delete to_del;
        }
    }
};

bool can_be_placed(const BarSlot* s, Client* c) {
    for(int i = 0; i < 8; ++i) {
        if(c->drinks[i] && !s->drinks_array[i]) {
            return false;
        }
    }
    return true;
}

bool place(BarSlot* bar, Client* clients) {
    BarSlot* current_begin = bar;
    int current_distance = 0;
    Client* current_client_iter = clients;
    for(;current_begin; current_begin = current_begin->next) {
        for(BarSlot* iter = bar; iter->next; iter = iter->next) {
            if(!can_be_placed(iter, current_client_iter)) {
                continue;
            }
        }
    }
}
int main() try {

    return 0;
} catch(const std::exception& e) {
    std::cerr << "the following error occurred: ";
    std::cerr << e.what() << '\n';
    return -1;
}