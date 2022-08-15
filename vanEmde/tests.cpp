#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "van Emde Boas.hpp"
#include <climits>
#include <utility>

static constexpr int size = 1 << 8;
static const std::vector<int> sample_numbers{ 4, 72, 14, 6, 5, 3, 168, 42 };
static const std::vector<int> sample_numbers1{ 27, 63, 15, 237, 45, 133, 17, 171};

#if 1

void init_tree(vanEmde_tree& t, const std::vector<int>& vec) {
    for(int num : vec) {
        t.insert(num);
    }
}

TEST_CASE("Creation of an empty tree", "[vanEmde_tree]") {
    vanEmde_tree t;
    REQUIRE(t.empty());
    REQUIRE(!t.size());
    REQUIRE(!t.contains(42));
}

TEST_CASE("Insertion works correctly", "[vanEmde_tree]") {
    vanEmde_tree t(size);
    init_tree(t, sample_numbers);

    SECTION("For input in universe") {
        for(int num : sample_numbers) {
            REQUIRE(t.contains(num));
        }

        REQUIRE(!t.contains(123));

        REQUIRE(!t.empty());
        REQUIRE(t.universe_size() == size);
        REQUIRE(t.size() == sample_numbers.size());

        REQUIRE(t.max() == 168);
        REQUIRE(t.min() == 3);
    }

    SECTION("For input outside of universe") {
        REQUIRE_THROWS(t.insert(size));
        REQUIRE_THROWS(t.insert(-1));
    }
}

TEST_CASE("Predecessor/Successor work correctly", "[vanEmde_tree]") {
    vanEmde_tree t(size);
    init_tree(t, sample_numbers);

    std::vector<int> sorted(sample_numbers);
    std::ranges::sort(sorted);

    SECTION("Predecessor") {
        REQUIRE(t.predecessor(2) == vanEmde_tree::None);
        REQUIRE(t.predecessor(3) == vanEmde_tree::None);

        for(int i = 1; i < sorted.size(); ++i) {
            REQUIRE(t.predecessor(sorted[i]) == sorted[i - 1]);
        }
        REQUIRE(t.predecessor(INT_MAX) == sorted.back());
        REQUIRE(t.predecessor(INT_MIN) == vanEmde_tree::None);
        REQUIRE(t.predecessor(sorted.front()) == vanEmde_tree::None);
    }

    SECTION("Successor") {
        for(int i = 0; i < sorted.size() - 1; ++i) {
            REQUIRE(t.successor(sorted[i]) == sorted[i + 1]);
        }
        REQUIRE(t.successor(INT_MIN) == sorted.front());
        REQUIRE(t.successor(INT_MAX) == vanEmde_tree::None);
        REQUIRE(t.successor(sorted.back()) == vanEmde_tree::None);
    }
}


TEST_CASE("Erasure works correctly", "[vanEmde_tree]") {
    vanEmde_tree t(size);
    init_tree(t, sample_numbers);

    int sz = sample_numbers.size();

    REQUIRE(t.size() == sz);

    SECTION("For input in Universe") {
        for(int num : sample_numbers) {
            t.erase(num);
            --sz;
            REQUIRE(t.size() == sz);
            REQUIRE(!t.contains(num));
        }
    }

    SECTION("For input outside of Universe") {
        REQUIRE_THROWS(t.erase(169));
        REQUIRE_THROWS(t.erase(INT_MIN));
    }
}

TEST_CASE("Copy semantics works correctly", "[vanEmde_tree]") {
    vanEmde_tree t1(size);
    init_tree(t1, sample_numbers);

    SECTION("Copy ctor") {
        vanEmde_tree t2(t1);

        REQUIRE(t2.size() == t1.size());
        REQUIRE(t2.universe_size() == t1.universe_size());

        for(int elem : sample_numbers) {
            REQUIRE(t2.contains(elem));
        }
    }

    SECTION("Copy assignment") {
        vanEmde_tree t2(size);
        init_tree(t2, sample_numbers1);

        t2 = t1;

        REQUIRE(t2.size() == t1.size());
        REQUIRE(t2.universe_size() == t1.universe_size());

        for(int elem : sample_numbers) {
            REQUIRE(t2.contains(elem));
        }
    }
}

TEST_CASE("Move semantics works correctly", "vanEmde_tree") {
    vanEmde_tree t1(size);
    init_tree(t1, sample_numbers1);

    SECTION("Move ctor") {
        vanEmde_tree t2(std::move(t1));

        REQUIRE(t2.size() == sample_numbers1.size());
        for(int num : sample_numbers1) {
            REQUIRE(t2.contains(num));
        }
    }

    SECTION("Move assignment") {
        vanEmde_tree t2(size);
        init_tree(t2, sample_numbers);
        t2 = std::move(t1);
        REQUIRE(t2.size() == sample_numbers1.size());
        for(int num : sample_numbers1) {
            REQUIRE(t2.contains(num));
        }
        REQUIRE(t1.empty());
    }
}

#endif

#if 0
#include <iostream>
#include <stdexcept>
#include <cassert>

int main() try {

    vanEmde_tree t(256);

    t.insert(3);
    assert(tree.contains(3));
    t.insert(4);
    t.insert(5);
    t.insert(6);
    t.insert(14);
    t.insert(42);
    t.insert(72);
    t.insert(168);

    assert(t.contains(4));
    assert(t.contains(5));
    assert(t.contains(6));
    assert(t.contains(14));
    assert(t.contains(42));
    assert(t.contains(72));
    assert(t.contains(168));

    std::cout << t.successor(3);
/*
    assert(t.successor(INT_MIN) == 3);
    assert(t.successor(3) == 4);
    assert(t.successor(4) == 5);
    assert(t.successor(5) == 6);
    assert(t.successor(6) == 14);
    assert(t.successor(14) == 42);
    assert(t.successor(42) == 72);
    assert(t.successor(72) == 168);
*/
/*
    assert(tree.contains(5));
    assert(tree.contains(7));
    assert(tree.contains(12));
    assert(tree.contains(14));
*/
    std::cout << std::endl;
/*
    vanEmde_tree cp = tree;

    assert(cp.contains(5));
    assert(cp.contains(7));
    assert(cp.contains(12));
    assert(cp.contains(14));

*/
    return 0;
} catch(const std::exception& e) {
    std::cerr << "Something went horribly wrong: \n";
    std::cerr << e.what() << '\n';
}

#endif