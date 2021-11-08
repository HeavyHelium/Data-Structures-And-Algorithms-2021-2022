#pragma once
#include <vector>

class dynamic_stack
{
    std::vector<int> data;
public:
    void push(int value)
    { data.push_back(value); }

    void pop(int value)
    { data.pop_back(); }

    int& top()
    { return data.back(); }

    const int& top() const
    { return data.back(); }

    std::size_t size() const
    { return data.size(); }

    bool empty() const
    { return data.empty(); }

    void shrink_to_fit()
    { data.shrink_to_fit(); }
};
