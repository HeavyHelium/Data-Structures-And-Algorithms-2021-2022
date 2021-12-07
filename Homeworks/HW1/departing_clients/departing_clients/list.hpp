#pragma once
#ifndef _LIST_HEADER_INCLUDED_
#define _LIST_HEADER_INCLUDED_
#include <cassert>
#include <initializer_list>
#include <algorithm>

/// @brief simple doubly-linked list implementation
/// @tparam T 
template<typename T>
class list
{
    struct Node;
public:
    ///TODO: use SFINAE and avoid code duplication
    class const_iterator
    {
        friend class list;
        const Node* ptr;
        const_iterator(Node* ptr) : ptr(ptr) {}
        const_iterator& operator++() { ptr = ptr->next; return *this; }
        const_iterator operator++(int) { const_iterator copy(*this); ++(*this); return copy; }
        const_iterator& operator--() { ptr = ptr->prev; return *this; };
        const_iterator operator--(int) { const_iterator copy(*this); --(*this); return copy; }
        const T& operator*() { return ptr->data; }
        const T* operator->() { return &(ptr->data); }
        bool operator==(const const_iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const const_iterator& other) const { return !(*this == other); }
    };
    class iterator 
    {
        friend class list;
        Node* ptr;
    public:
        iterator(Node* ptr) : ptr(ptr) {}
        iterator& operator++() { ptr = ptr->next; return *this; }
        iterator operator++(int) { iterator copy(*this); ++(*this); return copy; }
        iterator& operator--() { ptr = ptr->prev; return *this; };
        iterator operator--(int) { iterator copy(*this); --(*this); return copy; }
        T& operator*() { return ptr->data; }
        T* operator->() { return &(ptr->data); }
        bool operator==(const iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const iterator& other) const { return !(*this == other); }
        operator const_iterator() const { return const_iterator(ptr); }
    };
    
    list() = default;
    list(const std::initializer_list<T>& initlist);
    
    // Rule of 5
    list(const list& other);
    list(list&& other);
    /// @brief copy-and-swap idiom
    /// @param other list
    /// @return list&
    list& operator=(list other);
    ~list();

    void push_front(const T& elem);
    void push_back(const T& elem);
    void pop_front();
    void pop_back();

    T& front() 
    { return first -> data; }
    const T& front() const 
    { return last -> data; }
    T& back()  
    { return first-> data; }
    const T& back()  const 
    { return first -> data; }
    std::size_t size() const 
    { return m_size; }
    bool empty() const 
    { return m_size == 0; }
    
    void clear();    

    // Iterator methods
    iterator begin() 
    { return iterator(first); }
    const_iterator begin() const 
    { return iterator(first); }

    iterator end() 
    { return iterator(nullptr); }
    const_iterator end() const 
    { return iterator(nullptr); }

    void insert(iterator position, const T& elem);
    void erase(iterator position);

private:
    struct Node
    {
        T  data;
        Node* prev;
        Node* next;

        Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
            : data(data), prev(prev), next(next)
        {}
    };
    Node* first = nullptr;
    Node* last = nullptr;
    std::size_t m_size = 0;
};

//========================Implementation of methods========================

template<typename T>
list<T>::list(const std::initializer_list<T>& initlist)
{
    for(const T& elem : initlist)
        push_back(elem);
}

template<typename T>
list<T>::list(const list& other)
{
    for (const T& elem : other)
        push_back(elem);
}

template<typename T>
list<T>::list(list&& other)
{
    std::swap(first, other.first);
    std::swap(last, other.last);
    std::swap(m_size, other.m_size);
}

template<typename T>
list<T>& list<T>::operator=(list other)
{
    std::swap(first, other.first);
    std::swap(last, other.last);
    std::swap(m_size, other.m_size);
    return *this;
}

template<typename T>
list<T>::~list() { clear(); }

template<typename T>
void list<T>::push_front(const T& elem)
{
    if (first == nullptr)
        last = first = new Node(elem);
    else
        first = first->prev = new Node(elem, nullptr, first);
    ++m_size;
}

template<typename T>
void list<T>::push_back(const T& elem)
{
    if (first == nullptr)
        last = first = new Node(elem);
    else
        last = last->next = new Node(elem, last, nullptr);
    ++m_size;
}

template<typename T>
void list<T>::pop_front()
{
    assert(!empty() && "cannot pop from an empty list");
    if (first == last)
    {
        delete first;
        last = first = nullptr;
    }
    else
    {
        first = first->next;
        delete first->prev;
        first->prev = nullptr;
    }
    --m_size;
}

template<typename T>
void list<T>::pop_back()
{
    assert(!empty() && "cannot pop from an empty list");
    if (first == last)
    {
        delete first;
        last = first = nullptr;
    }
    else
    {
        last = last->prev;
        delete  last->next;
        last->next = nullptr;
    }
    --m_size;
}

template<typename T>
void list<T>::clear() { while (!empty()) pop_back(); }

template<typename T>
void list<T>::insert(iterator position, const T& elem)
{
    if (position == end()) { push_back(elem); return; }
    if (position == this->begin()) { push_front(elem); return; }

    Node* current = position.ptr;
    Node* previous = current->prev;
    Node* new_node = new Node(elem, previous, current);

    current->prev = new_node;
    previous->next = new_node;

    ++m_size;
}

template<typename T>
void list<T>::erase(iterator position)
{
    assert(!empty() && "cannot erase elements from an empty list");
    if (position == begin()) { pop_front(); return; }
    if (position == end()) { pop_back(); return; }

    Node* current = position.ptr;
    Node* previous = current->prev;
    Node* next = current->next;

    previous->next = next;
    next->prev = previous;

    delete current;
    --m_size;
}

#endif // !_LIST_HEADER_INCLUDED_
