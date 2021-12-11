#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP
#include "../forward_list/forward_list.hpp"
template<class T>
class priority_queue
{
    forward_list<T> lst;
public:
    priority_queue(){}
    void push(const T& element);
    void pop();
    T& top(){ *lst.begin(); }
    bool empty() { return lst.empty(); }
    std::size_t size() { return lst.size(); }
    void erase(iterator pos)
    { lst.erase(pos); }
    /// TODO!!!: remove those
    using iterator = typename forward_list<T>::iterator;
    iterator begin() { return lst.begin(); }
    iterator end() { return lst.end(); }
};

template<typename T>
void priority_queue<T>::push(const T& element)
{
    if(lst.empty()) lst.insert(lst.begin(), element);
    else if(lst.size() >= 1 && *lst.begin() >= element) lst.push_front(element);
    else
    {
        typename forward_list<T>::iterator prev = lst.begin();

        for(typename forward_list<T>::iterator iter = ++lst.begin(); iter != lst.end() && *iter < element; ++iter)
        { ++prev; }
        lst.insert_after(prev, element);
    }
}

template<typename T>
void priority_queue<T>::pop()
{
    assert("cannot pop elements from an empty queue" && !empty());
    lst.pop_front();
}




#endif // PRIORITY_QUEUE_HPP
