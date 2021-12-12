#pragma once
#ifndef LIST_TESTS_H
#define LIST_TESTS_H

#include "list.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
TEST_CASE("Objects are well-constructed initially", "[list]")
{
    list<int> l1;
    REQUIRE(l1.size() == 0);
}

TEST_CASE("Insertion works properly", "[list]")
{
    list<int> l1;
    SECTION("push_front to an empty list works")
    {
        l1.push_front(5);
        REQUIRE(l1.size() == 1);
        REQUIRE(*l1.begin() == 5);
        l1.push_front(4);
        REQUIRE(l1.size() == 2);
        REQUIRE(*l1.begin() == 4);
        REQUIRE(*(++l1.begin()) == 5);
    }
    SECTION("push_back to an empty list works")
    {
        l1.push_back(5);
        REQUIRE(l1.size() == 1);
        REQUIRE(*l1.begin() == 5);
        l1.push_back(4);
        REQUIRE(l1.size() == 2);
        REQUIRE(*++l1.begin() == 4);
        l1.push_back(3);
        REQUIRE(l1.size() == 3);
        REQUIRE(*++++l1.begin() == 3);
    }
    SECTION("insertion at a given position works")
    {
        l1.push_front(5);
        REQUIRE(l1.size() == 1);
        REQUIRE(*l1.begin() == 5);
        l1.push_front(4);
        REQUIRE(l1.size() == 2);
        REQUIRE((*l1.begin() == 4));
        REQUIRE(*(++l1.begin()) == 5);
        l1.insert(l1.begin(), 6);
        REQUIRE(l1.size() == 3);
        REQUIRE(*l1.begin() == 6);
        l1.insert(++l1.begin(), 7);
        REQUIRE(l1.size() == 4);
        REQUIRE(*(++l1.begin()) == 7);
        REQUIRE(*(++l1.begin()) == 7);
    }
}

TEST_CASE("Removal works properly", "[list]")
{
    SECTION("Removal before works properly")
    {
        list<int> fl;
        for (int i = 9; i >= 0; --i)
            fl.push_front(i);
        fl.pop_front();
        REQUIRE(fl.size() == 9);
        fl.erase(++fl.begin());
        REQUIRE(fl.size() == 8);
        REQUIRE(*fl.begin() == 1);
        REQUIRE(*(++fl.begin()) == 3);
    }
}

TEST_CASE("Copy semantics works properly", "[list]")
{
    SECTION("Copy construction")
    {
        list<int> fl;
        fl.push_front(11);
        fl.push_front(10);
        fl.push_front(9);
        fl.push_front(8);
        fl.push_front(7);
        fl.push_front(6);
        fl.push_front(5);
        fl.insert(fl.end(), 17);
        list<int> f2(fl);
        list<int>::iterator iter = f2.begin();
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
        list<int> fl;
        fl.push_front(11);
        fl.push_front(10);
        fl.push_front(9);
        fl.push_front(8);
        fl.push_front(7);
        fl.push_front(6);
        fl.push_front(5);
        fl.insert(fl.end(), 17);
        list<int> f2;
        f2.push_front(42);
        f2 = fl;
        list<int>::iterator iter = f2.begin();
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
#endif
