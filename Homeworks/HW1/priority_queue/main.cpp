#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "priority_queue.hpp"

TEST_CASE("Initial creation", "[priority_queue]")
{
    priority_queue<int> q;
    REQUIRE(q.size() == 0);
}

TEST_CASE("Insertion of elements is done correctly", "[priority_queue]")
{
    priority_queue<int> q;
    q.push(5);
    q.push(7);
    q.push(3);
    q.push(6);
    q.push(2);
    q.push(1);
    q.push(4);
    typename priority_queue<int>::iterator iter = q.begin();
    REQUIRE(*iter++ == 1);
    REQUIRE(*iter++ == 2);
    REQUIRE(*iter++ == 3);
    REQUIRE(*iter++ == 4);
    REQUIRE(*iter++ == 5);
    REQUIRE(*iter++ == 6);
    REQUIRE(*iter == 7);
}

TEST_CASE("Popping of elements is done correctly", "[priority_queue]")
{
    priority_queue<int> q;
    q.push(5);
    q.push(7);
    q.push(3);
    q.push(6);
    q.push(2);
    q.push(1);
    q.push(4);
    q.pop();
    REQUIRE(*q.begin() == 2);
    q.pop();
    REQUIRE(*q.begin() == 3);
    q.pop();
    REQUIRE(*q.begin() == 4);
    q.pop();
    REQUIRE(*q.begin() == 5);
    q.pop();
    REQUIRE(*q.begin() == 6);
    q.pop();
    REQUIRE(*q.begin() == 7);
}

