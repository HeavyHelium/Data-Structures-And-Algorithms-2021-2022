#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../forward_list/forward_list.hpp"
#include <cassert>

template<typename T>
class queue
{
    forward_list<T> lst;
public:
    queue(){}
    // we push at the end
    void push(const T& element)
    { lst.push_back(element); }
    void pop()
    {
        assert("queue should be non-empty" && !empty());
        lst.pop_front();
    }
    std::size_t size() const { return lst.size(); }
    bool empty() const { return lst.empty(); }
    T& front()
    {
        assert("queue should be non-empty" && !empty());
        return *lst.begin();
    }
    const T& front() const
    {
        assert("queue should be non-empty" && !empty());
        return *lst.begin();
    }
};


#endif // QUEUE_HPP
