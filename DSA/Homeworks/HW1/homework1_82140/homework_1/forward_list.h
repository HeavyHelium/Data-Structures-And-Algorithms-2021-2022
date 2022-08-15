#pragma once
#ifndef FORWARD_LIST_HPP
#define FORWARD_LIST_HPP
#include <cassert>
#include <cstddef>
#include <stdexcept>

/// simple implementation of a singly-linked list
template<typename T>
class forward_list
{
    struct Node
    {
        T data;
        Node* next = nullptr;
        Node(const T& data, Node* ptr = nullptr) : data(data), next(ptr)
        {}
    };
public:
    class iterator
    {
        friend class forward_list;
        Node* ptr;
    public:
        iterator(Node* ptr) : ptr(ptr) {}
        bool operator!=(const iterator& other) const;
        iterator& operator++();
        iterator operator++(int);
        T* operator->() const;
        T& operator*() const;
    };
    class const_iterator
    {
        friend class forward_list;
        const Node* ptr;
    public:
        const_iterator(Node* ptr) : ptr(ptr) {}
        bool operator!=(const const_iterator& other) const;
        const_iterator& operator++();
        const_iterator operator++(int);
        const T* operator->() const;
        const T& operator*() const;
    };
    /// Rule of three
    forward_list();
    forward_list(const forward_list& other);
    /// copy-and-swap
    forward_list& operator=(forward_list other);
    ~forward_list();

    std::size_t size() const;
    bool empty() const;

    void insert(iterator pos, const T& data);
    void erase(iterator pos);

    void insert_after(iterator pos, const T& data);
    void erase_after(iterator pos);

    void pop_front();
    void push_front(const T& data);
    void push_back(const T& data);

    iterator begin() { return iterator(head); }
    const_iterator begin() const { return iterator(head); }
    /// returns iterator to past the last element
    iterator end() { return iterator(tail->next); }
    const_iterator end() const { return iterator(tail->next); }

private:
    Node* head = nullptr;
    Node* tail = nullptr;
    std::size_t m_size = 0;
    struct beg_end_pair { Node* beg; Node* end; };
    static beg_end_pair clone_chain(Node* start);
    static void free_chain(Node* start);
    void insert_before(Node* pos, const T& data);
    void remove_at(Node* pos);
};

template<typename T>
typename forward_list<T>::iterator& forward_list<T>::iterator::operator++()
{
    ptr = ptr->next;
    return *this;
}

template<typename T>
typename forward_list<T>::iterator forward_list<T>::iterator::operator++(int)
{
    typename forward_list<T>::iterator copy(*this);
    ++(*this);
    return copy;
}

template<typename T>
bool forward_list<T>::iterator::operator!=(const typename forward_list<T>::iterator& other) const { return ptr != other.ptr; }

template<typename T>
T& forward_list<T>::iterator::operator*() const { return ptr->data; }

template<typename T>
T* forward_list<T>::iterator::operator->() const { return &(ptr->data); }


template<typename T>
typename forward_list<T>::const_iterator& forward_list<T>::const_iterator::operator++()
{
    ptr = ptr->next;
    return *this;
}

template<typename T>
typename forward_list<T>::const_iterator forward_list<T>::const_iterator::operator++(int)
{
    typename forward_list<T>::const_iterator copy(*this);
    ++(*this);
    return copy;
}

template<typename T>
bool forward_list<T>::const_iterator::operator!=(const typename forward_list<T>::const_iterator& other) const { return ptr != other.ptr; }

template<typename T>
const T& forward_list<T>::const_iterator::operator*() const { return ptr->data; } 

template<typename T>
const T* forward_list<T>::const_iterator::operator->() const { return &(ptr->data); }


/// Rule of three
template<typename T>
forward_list<T>::forward_list() {}

template<typename T>
forward_list<T>::forward_list(const forward_list& other)
{
    if (!other.empty())
    {
        beg_end_pair temp = clone_chain(other.head);
        head = temp.beg;
        tail = temp.end;
        m_size = other.m_size;
    }
}

/// copy-and-swap-idiom
template<typename T>
forward_list<T>& forward_list<T>::operator=(forward_list other)
{
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(m_size, other.m_size);
    return *this;
}

template<typename T>
forward_list<T>::~forward_list() { free_chain(head); }

template<typename T>
std::size_t forward_list<T>::size() const { return m_size; }

template<typename T>
bool forward_list<T>::empty() const { return m_size == 0; }

template<typename T>
void forward_list<T>::insert(iterator pos, const T& data)
{
    insert_before(pos.ptr, data);
    ++m_size;
}

template<typename T>
void forward_list<T>::erase(iterator pos)
{
    assert(!empty());
    remove_at(pos.ptr);
    --m_size;
}

template<typename T>
void forward_list<T>::pop_front() { erase(begin()); }

template<typename T>
void forward_list<T>::push_front(const T& data) { insert(begin(), data); }

template<typename T>
void forward_list<T>::push_back(const T& data) { insert_after(iterator(tail), data); }

template<typename T>
typename forward_list<T>::beg_end_pair forward_list<T>::clone_chain(Node* start)
{
    if (!start) return beg_end_pair{ nullptr, nullptr };
    Node* result = nullptr;
    Node* current = nullptr;
    try
    {
        result = new Node(start->data);
        current = result;
        while (start = start->next)
        {
            current->next = new Node(start->data);
            current = current->next;
        }
    }
    catch (...)
    {
        free_chain(result); throw;
    }
    return { result, current };
}

template<typename T>
void forward_list<T>::free_chain(Node* start)
{
    while (start)
    {
        Node* temp = start;
        start = start->next;
        delete temp;
    }
}

template<typename T>
void forward_list<T>::insert_before(Node* pos, const T& data)
{
    if (empty()) tail = head = new Node(data);
    else
    {
        Node* prev = head;
        if (prev == pos)
        {
            head = new Node(data, head); if (empty()) tail = head;
        }
        else if (pos == tail->next)
        {
            tail = tail->next = new Node(data);
        }
        else
        {
            while (prev->next != pos)
            {
                prev = prev->next;
            }
            prev->next = new Node(data, prev->next);
        }
    }
}

template<typename T>
void forward_list<T>::remove_at(Node* pos)
{
    if (empty()) throw std::underflow_error("List is empty, cannot erase elements");
    if (pos == head)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
        if (!head) tail = nullptr;
    }
    else
    {
        Node* prev = head;
        while (prev->next != pos)
        {
            prev = prev->next;
        }
        Node* detach = prev->next;
        prev->next = detach->next;
        if (detach == tail) tail = prev;
        delete detach;
    }
}

template<typename T>
void forward_list<T>::insert_after(iterator pos, const T& data)
{
    if (empty()) tail = head = new Node(data);
    else if (pos.ptr == tail) tail = tail->next = new Node(data);
    else
        pos.ptr->next = new Node(data, pos.ptr->next);
    ++m_size;
}

template<typename T>
void forward_list<T>::erase_after(iterator pos)
{
    assert("list should be non-empty when erasing elements" && !empty());
    assert(head != tail);
    pos.ptr->next = pos.ptr->next->next;
    --m_size;
}


#endif // FORWARD_LIST_HPP


