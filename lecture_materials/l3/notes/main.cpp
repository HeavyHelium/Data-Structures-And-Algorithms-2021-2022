/**
*
* Notes for lecture 3 for SDP course
* on 25.10.2021
*
*
*/
#include <iostream>
#include <cstddef>
#include <exception>
#include <cassert>

template<size_t Size>
class simple_stack
{
    int data[Size]; // fixed size
    std::size_t used = 0;
public:
    std::size_t size() const { return Size; }

    void push(int value)
    {
        assert(used < Size && "stack is full");
        data[used++] = value;
    }

    void pop()
    {
        assert(used > 0 && "stack is empty");
        --used;
    }

    int& top()
    {
        assert(used > 0);
        return data[used - 1];
    }
    const int& top() const
    {
        assert(used > 0);
        return data[used - 1];
    }
    bool empty() const  { return used == 0; }
};




//class simple_stack
//{
//    int data[100]; // fixed size
//    std::size_t used = 0;
//public:
//    std::size_t size() const { return 100; }
//
//    void push(int value)
//    {
//        assert(used < 100 && "stack is full")
//        data[used++] = value;
//    }
//
//    void pop()
//    {
//        assert(used > 0 && "stack is empty");
//        --used;
//    }
//
//    int& top()
//    {
//        assert(used > 0);
//        return data[used - 1];
//    }
//    const int& top() const
//    {
//        assert(used > 0);
//        return data[used - 1];
//    }
//};

/// Look at godbolt.org

int main()
{
    /// these are simple_stacks of different types
    simple_stack<100> s1;
    //simple_stack<200> s2;

    simple_stack<100>* p = new simple_stack<100'000'000>();

    /// not allowed!
    // s1 = s2;

    /// most likely the methods are going to be inlined
    std::cout << s1.size() << std::endl;
    s1.push(42);

    for(int i = 0; i < 5; ++i)
    {
        int input;
        std::cin >> input;
        s1.push(input);
    }

    while(!s1.empty())
    {
        std::cout << s1.top() << '\n';
        s1.pop();
    }

    return 0;
}
