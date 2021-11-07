#include <iostream>
#include <initializer_list>
#include <list>
#include <utility>
#include <stdexcept>
#include <cstddef>
#include <cassert>

template<class T>
class linked_list
{


public:
    struct node
    {
        T data;
        T* next = nullptr;
        bool has_successor() const
        { return next == nullptr; }
    };

private:
    node* m_begin = nullptr;
    node* m_end = nullptr;
    std::size_t m_size = 0;

public:
   class iterator
   {
       node* start;
       node* current;
    public:
        iterator(node* start)
            : start(start), current(start)
        {}
        T& operator*() { return current->data; }
        const T& operator*() const { return current->data; }
        T& operator->() { return &(current->data); }
        const T& operator->() const { return &(current->data); }
        iterator& operator++() { current = current -> next; return *this; }
        iterator& operator++(int) { iterator copy(this); ++(*this); return copy; }
        iterator& operator+=(std::size_t offset)
        {
            for(std::size_t i = 0; i < offset; ++i)
                current = current -> next;
            return *this;
        }
        iterator operator+(std::size_t offset)
        {
            iterator copy = *this;
            return copy += offset;
        }
        bool operator==(const iterator& other) const
        { return current == other.current; }
        bool operator!=(const iterator& other) const
        { return !(*this == other); }
        operator bool() const { return current; }
        void rewind() { current = start; }
        bool reached_end() const { return current == nullptr; }


   };

    linked_list() = default;
    linked_list(const linked_list& other) noexcept
    {
        copy(other);
    }
    linked_list(linked_list&& other)
    {
        std::swap(m_begin, other.m_begin);
        std::swap(m_end, other.m_end);
        std::swap(m_size, other.m_size);
    }
    linked_list& operator=(const linked_list& other)
    {
        if(this != &other)
        {
            clear();
            copy(other);
        }
        return *this;
    }
    linked_list& operator=(linked_list&& other)
    {
        std::swap(m_begin, other.m_begin);
        std::swap(m_end, other.m_end);
        std::swap(m_size, other.m_size);
        return *this;
    }

    void push_front(const T& element)
    {
        node* new_node = new node{ element, m_begin };
        if(empty()) m_end = m_begin;
        m_begin = new_node;
        ++m_size;
    }

    void push_front(T&& element)
    {
        node* new_node = new node{ std::move(element), m_begin };
        if(empty()) m_end = m_begin;
        m_begin = new_node;
        ++m_size;
    }

    void push_at(std::size_t pos, const T& value)
    {
        if(pos > m_size)
            throw std::out_of_range("Index exceeds the number of elements in the list");
        if(pos == 0)
            push_front(value);
        else if(pos == m_size)
            push_back(value);
        else
        {
            node* previous_node = find_node(pos - 1);
            previous_node -> next = new node { value, previous_node -> next };
            ++m_size;
        }
    }

    void insert(std::size_t pos, T&& value)
    {
        if(pos > m_size)
            throw std::out_of_range("Index exceeds the number of elements in the list");
        if(pos == 0)
            push_front(std::move(value));
        else if(pos == m_size)
            push_back(std::move(value));
        else
        {
            node* previous_node = find_node(pos - 1);
            previous_node -> next = new node { std::move(value), previous_node -> next };
            ++m_size;
        }
    }

    void push_back(const T& element)
    {
        node* new_node = new node{ element, nullptr };
        if(!empty()) m_end = m_end -> next = new_node;
        else { m_end = m_begin = new_node; }
        ++m_size;
    }

    void push_back(T&& element)
    {
        node* new_node = new node{ std::move(element), nullptr };
        if(!empty()) m_end = m_end -> next = new_node;
        else { m_end = m_begin = new_node; }
        ++m_size;
    }

    void pop_front()
    {
        if(empty()) throw std::underflow_error("Cannot pop off an empty list!");
        node* to_delete = m_begin;
        m_begin = m_begin -> next;
        delete to_delete;
        if(m_begin == nullptr)
            m_end = nullptr;
        --m_size;
    }

    void pop_back()
    {
        if(empty()) throw std::underflow_error("Cannot pop off an empty list!");
        erase(m_size - 1);
    }

    void erase(std::size_t index)
    {
        if(index > m_size)
            throw std::out_of_range("Index exceeds the number of elements int the list");
        if(index == 0)
            pop_front();
        else
        {
            assert(m_size >= 2); // with the previous checks it must be true for sure
            node* previous_node = find_node(index - 1);
            assert(previous_node != nullptr);
            node* current_node = previous_node -> next;
            if(index == m_size - 1)
                m_end = previous_node;
            previous_node -> next = current_node->next;
            delete current_node;
            --m_size;
        }

    }

    iterator begin()
    { return { m_begin }; }

    iterator end()
    { return { m_end }; } // m_end might be nullptr

    bool empty() const { return m_size == 0; }
    std::size_t size() const { return m_size; }

private:
    node* find_node(std::size_t index)
    {
        node* ptr = m_begin;
        for(std::size_t i = 0; i != index; ++i, (ptr = (ptr -> next)))
            ;
        return ptr;
    }
    void copy(const linked_list& other)
    {
        node temp{ T{}, nullptr };
        node* iter = & temp;
        node* current = other.m_begin;
        while(current)
        {
            iter = iter -> next = new node{ current -> data, nullptr };
            current = current -> next;
        }
        m_begin = temp.next;
        m_end = m_begin ? iter : nullptr;
        m_size = other.m_size;

    }

    void copy(linked_list&& other)
    {
        node temp{ T{}, nullptr };
        node* iter = & temp;
        node* current = other.m_begin;
        while(current)
        {
            iter = iter -> next = new node{ std::move(current -> data), nullptr };
            current = current -> next;
        }
        m_begin = temp.next;
        m_end = m_begin ? iter : nullptr;
        m_size = other.m_size;

    }

    void clear()
    {
        node* current = m_begin;
        while(current)
        {
            node* temp = current;
            current = current -> next;
            delete temp;
        }
        m_begin = m_end = nullptr; // just in case
    }

};







int main()
{

    return 0;
}


















////
////struct apple
////{
////    double w = 5.23;
////    char quality = 'a';
////};
/////*
////struct weight
////{
////    double w;
////    weight(double wei)
////        : w(wei)
////    { std::cout << "weight ctor with: " << w << "\n"; }
////};
////*/
////class taciturn_apple
////{
////    double w = 12;
////    int t = -1;
////public:
////
////    taciturn_apple(double wei)
////        : w(13)
////    {std::cout << "ctor with w: " << w << "\n";}
////    taciturn_apple()
////    { std::cout << "def ctor\n"; }
////    taciturn_apple(std::initializer_list)
////    double get_w() { return w; }
////};
