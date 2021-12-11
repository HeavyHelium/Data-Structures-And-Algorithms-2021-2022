#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "queue.hpp"


TEST_CASE("empty queues are created properly", "[queue]")
{
    queue<int> q;
    REQUIRE(q.empty());
    REQUIRE(q.size() == 0);
}

TEST_CASE("Pushing works properly", "[queue]")
{
    queue<int> q;
    q.push(5);
    REQUIRE(q.size() == 1);
    REQUIRE(q.front() == 5);
    q.push(6);
    REQUIRE(q.size() == 2);
    REQUIRE(q.front() == 5);
}

TEST_CASE("Popping works properly", "[queue]")
{
    queue<int> q;
    q.push(5);
    REQUIRE(q.size() == 1);
    REQUIRE(q.front() == 5);
    q.push(6);
    q.pop();
    REQUIRE(q.size() == 1);
    REQUIRE(q.front() == 6);
    q.push(7);
    q.push(8);
    q.push(9);
    REQUIRE(q.size() == 4);
    q.pop();
    REQUIRE(q.front() == 7);
    q.pop();
    REQUIRE(q.front() == 8);
    q.pop();
    REQUIRE(q.front() == 9);
    q.pop();
    REQUIRE(q.empty());
}

