#include <iostream>
#include "forward_list.hpp"
#include <cassert>

int main()
{
    {
        forward_list<int> lst;
//        lst.push_front(3);
//        lst.push_front(2);
        lst.push_front(1);
        lst.push_front(0);
        lst.erase_after(lst.begin());
        for(auto element : lst)
            std::cout << element << " ";
        std::cout << std::endl;

    }

    {
        forward_list<int> lst;
        //lst.push_front(3);
        //lst.push_front(2);
        //lst.push_front(1);
        lst.push_front(0);
        lst.insert_after(lst.begin(), 7);
        for(auto element : lst)
            std::cout << element << " ";
        std::cout << std::endl;

    }

    {
        std::cout << "tests: " << std::endl;
        forward_list<int> l1;
        assert(l1.size() == 0);
        l1.push_front(5);
        assert(l1.size() == 1);
        *l1.begin() == 5;
        l1.push_front(4);
        assert(l1.size() == 2);
        (*l1.begin() == 4);
        assert(*(++l1.begin()) == 5);
        l1.insert(l1.begin(), 6);
        std::cout << "sz: " << l1.size() << std::endl;
        assert(l1.size() == 3);
        *l1.begin() == 6;
        l1.insert(++l1.begin(), 7);
        assert(l1.size() == 4);
        assert(*(++l1.begin()) == 7);
        assert(*(++l1.begin()) == 7);
    }

    {
        forward_list<int> fl;
        for(int i = 9; i >= 0; --i)
        fl.push_front(i);
        fl.pop_front();
        assert(fl.size() == 9);
        std::cout << "here\n\n";
        fl.erase(++fl.begin());
        assert(fl.size() == 8);
        assert(*fl.begin() == 1);
        assert(*(++fl.begin()) == 3);
    }
    forward_list<int> fl;
    fl.push_front(11);
    fl.push_front(10);
    fl.push_front(9);
    fl.push_front(8);
    fl.push_front(7);
    fl.push_front(6);
    fl.push_front(5);
    fl.insert(fl.end(), 17);
    for(auto elem : fl)
        std::cout << elem << " ";

    std::cout << std::endl;
    forward_list<int> f2(fl);
    for(auto elem: f2)
        std::cout << elem << std::endl;
    fl.pop_front();
    fl.pop_front();
    fl.pop_front();
    f2 = fl;
    for(auto elem: f2)
        std::cout << elem << std::endl;
    if(!fl.empty())
    for(auto elem : fl)
        std::cout << elem << " ";
    std::cout << fl.size() << std::endl;
        //fl.pop_front();

    return 0;
}
