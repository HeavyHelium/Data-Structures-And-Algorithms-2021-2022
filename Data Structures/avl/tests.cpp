#include "AVL.hpp"
#include <cmath>

#define CATCH_CONFIG_MAIN
#define CATCH_TESTS

int max_height(int node_cnt) {
    return std::floor(1.44 * std::log2(node_cnt + 1) - 0.328);
}

template<class T>
bool operator==(const std::vector<T>& v1, const std::vector<T>& v2) {
    if(v1.size() == v2.size()) {
        for(int i = 0; i < v1.size(); ++i) {
            if(v1[i] != v2[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

#if 0
TEST_CASE("sanity test") {
    REQUIRE((5 + 5 == 10));
}
#endif

#ifdef CATCH_TESTS
#include "catch.hpp"


TEST_CASE("Empty avl tree is created correctly", "[avl_tree]") {
    avl_tree<int> tree;
    REQUIRE(tree.size() == 0);
    REQUIRE(tree.empty());
    REQUIRE(!tree.contains(5));
}


TEST_CASE("Insertion in avl tree is performed correctly", "[avl_tree]") {
    SECTION("When inserting sorted sequence in descending order") {
        avl_tree<int> tree;
        tree.insert(8);
        tree.insert(7);
        tree.insert(6);
        tree.insert(5);
        REQUIRE(tree.size() == 4);
        REQUIRE(!tree.empty());
        REQUIRE(tree.contains(5));
        tree.insert(4);
        tree.insert(3);
        tree.insert(2);
        tree.insert(1); // if it were a naive BST implementation,
        // this would be a linked list(an expensive one)
        // with height 4
        REQUIRE(tree.size() == 8);
        REQUIRE(tree.height() <= max_height(tree.size()));
    }
    SECTION("When inserting sorted sequence in ascending order") {
        avl_tree<int> tree;
        tree.insert(1);
        tree.insert(2);
        tree.insert(3);
        tree.insert(4);
        tree.insert(5);
        tree.insert(6);
        tree.insert(7);
        tree.insert(8);
        REQUIRE(tree.size() == 8);
        REQUIRE(tree.height() <= max_height(tree.size()));
    }
    SECTION("When using initializer list construction") {
        avl_tree<int> tree{ 1, 5, 4, 2, 6, 42, 0, -18 };
        REQUIRE(tree.size() == 8);
        REQUIRE(tree.height() <= max_height(tree.size()));
    }
}

TEST_CASE("Information methods tell the truth", "[avl_tree]") {
    avl_tree<int> tree{ 1, 5, 4, 2, 6, 42, 0, -18 };
    SECTION("Size and weight methods") {
        REQUIRE(tree.size() == 8);
        REQUIRE(tree.height() <= max_height(tree.size()));
    }
    SECTION("Find method") {
        const avl_tree<int>::node* iter = tree.find(6);
        REQUIRE(iter);
        REQUIRE(iter->key == 6);
        REQUIRE(!tree.find(69));
    }
    SECTION("Contains method") {
        REQUIRE(tree.contains(6));
        REQUIRE(!tree.contains(69));
    }
    SECTION("Empty method") {
        REQUIRE(!tree.empty());
        tree.clear();
        REQUIRE(tree.empty());
    }
}

TEST_CASE("Erasure of elements is performed correctly", "[avl_tree]") {
    avl_tree<int> tree{ 1, 5, 4, 2, 6, 42, 0, -18 };
    tree.erase(42);
    REQUIRE(tree.size() == 7);
    REQUIRE_THROWS(tree.erase(69));
    tree.erase(1);
    REQUIRE(tree.height() <= max_height(tree.size()));
    tree.erase(5);
    tree.erase(2);
    tree.erase(4);
    tree.erase(6);
    REQUIRE(tree.height() <= max_height(tree.size()));
    tree.erase(0);
    tree.erase(-18);
    REQUIRE(tree.empty());
}

TEST_CASE("Inorder traversal is performed correctly", "[avl_tree]") {
    avl_tree<int> tree{ 5, 0, -42, 5, 85, 15, 14, 3 };
    std::vector<int> inorder = tree.inorder();
    std::vector<int> sorted{ -42, 0, 3, 5, 14, 15, 85 };
    REQUIRE(sorted.size() == inorder.size());
    for(int i = 0; i < sorted.size(); ++i) {
        REQUIRE(inorder[i] == sorted[i]);
    }
}

TEST_CASE("Copy semantics", "[avl_tree]") {
    avl_tree<int> tree{ 5, 0, -42, 5, 85, 15, 14, 3 };
    SECTION("Copy construction") {
        avl_tree<int> copy(tree);
        REQUIRE(tree.inorder() == copy.inorder());
    }
    SECTION("Copy assignment") {
        avl_tree<int> treeprime{ 42, 42, 42, 42, 42 };
        treeprime = tree;
        REQUIRE(tree.inorder() == treeprime.inorder());
    }
}

#endif

#ifndef CATCH_TESTS
#include <iostream>

int main() {
    avl_tree<int> tree{ 8, 7, 6, 5, 4, 3, 2, 1 };
    tree.erase(1);
    std::vector<int> inorder = tree.inorder();
    for(int elem : inorder) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    std::cout << "height: " << tree.height() << std::endl;
    std::cout << "weight: " << tree.size() << std::endl;
    return 0;
}
#endif
