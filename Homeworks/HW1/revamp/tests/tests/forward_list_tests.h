#pragma once
#ifndef FORWARD_LIST_TESTS_HPP_INCLUDED
#define FORWARD_LIST_TESTS_HPP_INCLUDED
#include "../../hw1/hw1/forward_list.h"


TEST_CASE("Objects are well-constructed when instantiatied", "[forward_list]")
{
    forward_list<int> l1;
    REQUIRE(l1.size() == 0);
}

TEST_CASE("Insertion works as meant", "[forward_list]")
{
    forward_list<int> l1;
    SECTION("push_front to an empty list works")
    {
        l1.push_front(5);
        REQUIRE(l1.size() == 1);
        *l1.begin() == 5;
        l1.push_front(4);
        REQUIRE(l1.size() == 2);
        (*l1.begin() == 4);
        REQUIRE(*(++l1.begin()) == 5);
    }
    SECTION("push_back to an empty list works")
    {
        l1.push_back(5);
        REQUIRE(l1.size() == 1);
        *l1.begin() == 5;
        l1.push_back(4);
        REQUIRE(l1.size() == 2);
        (*++l1.begin() == 4);
        l1.push_back(3);
        REQUIRE(l1.size() == 3);
        (*++++l1.begin() == 3);
    }
    SECTION("insertion at a given position works")
    {
        l1.push_front(5);
        REQUIRE(l1.size() == 1);
        *l1.begin() == 5;
        l1.push_front(4);
        REQUIRE(l1.size() == 2);
        (*l1.begin() == 4);
        REQUIRE(*(++l1.begin()) == 5);
        l1.insert(l1.begin(), 6);
        REQUIRE(l1.size() == 3);
        *l1.begin() == 6;
        l1.insert(++l1.begin(), 7);
        REQUIRE(l1.size() == 4);
        REQUIRE(*(++l1.begin()) == 7);
        REQUIRE(*(++l1.begin()) == 7);
    }
    SECTION("Insertion after works properly")
    {
        forward_list<int> lst;
        lst.push_front(1);
        lst.push_front(0);
        lst.insert_after(lst.begin(), 7);
        REQUIRE(*(++lst.begin()) == 7);
        REQUIRE(lst.size() == 3);
    }
}

TEST_CASE("Removal works as meant", "[forward_list]")
{
    SECTION("Removal before works properly")
    {
        forward_list<int> fl;
        for (int i = 9; i >= 0; --i)
            fl.push_front(i);
        fl.pop_front();
        REQUIRE(fl.size() == 9);
        fl.erase(++fl.begin());
        REQUIRE(fl.size() == 8);
        REQUIRE(*fl.begin() == 1);
        REQUIRE(*(++fl.begin()) == 3);
    }

    SECTION("Removal after works properly")
    {
        forward_list<int> lst;
        lst.push_front(1);
        lst.push_front(0);
        lst.erase_after(lst.begin());
        REQUIRE(lst.size() == 1);
        REQUIRE(*lst.begin() == 0);
    }
}

TEST_CASE("Copy semantics works", "[forward_list]")
{
    SECTION("Copy construction")
    {
        forward_list<int> fl;
        fl.push_front(11);
        fl.push_front(10);
        fl.push_front(9);
        fl.push_front(8);
        fl.push_front(7);
        fl.push_front(6);
        fl.push_front(5);
        fl.insert(fl.end(), 17);
        forward_list<int> f2(fl);
        forward_list<int>::iterator iter = f2.begin();
        REQUIRE(*iter == 5);
        REQUIRE(*(++iter) == 6);
        REQUIRE(*(++iter) == 7);
        REQUIRE(*(++iter) == 8);
        REQUIRE(*(++iter) == 9);
        REQUIRE(*(++iter) == 10);
        REQUIRE(*(++iter) == 11);
        REQUIRE(*(++iter) == 17);
    }

    SECTION("Copy assignment")
    {
        forward_list<int> fl;
        fl.push_front(11);
        fl.push_front(10);
        fl.push_front(9);
        fl.push_front(8);
        fl.push_front(7);
        fl.push_front(6);
        fl.push_front(5);
        fl.insert(fl.end(), 17);
        forward_list<int> f2;
        f2.push_front(42);
        f2 = fl;
        forward_list<int>::iterator iter = f2.begin();
        REQUIRE(*iter == 5);
        REQUIRE(*(++iter) == 6);
        REQUIRE(*(++iter) == 7);
        REQUIRE(*(++iter) == 8);
        REQUIRE(*(++iter) == 9);
        REQUIRE(*(++iter) == 10);
        REQUIRE(*(++iter) == 11);
        REQUIRE(*(++iter) == 17);
    }
}


#endif // FORWARD_LIST_TESTS_HPP_INCLUDED
