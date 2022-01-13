#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "sp_hash_table.hpp"
#include "string"

typedef std::string key_type;
typedef int value_type;

TEST_CASE("An empty hash is created correctly", "[sp_hash_table]") {
    sp_hash_table<key_type, value_type> t1;
    REQUIRE(t1.size() == 0);
    REQUIRE(t1.empty());
    REQUIRE(t1.bucket_count() == sp_hash_table<key_type, value_type>::INITIAL_BUCKET_COUNT);
    REQUIRE(!t1.contains("slartibarfast"));
    REQUIRE(t1.get_load_factor() == 0);
}

TEST_CASE("Insertion of items works correctly", "[sp_hash_table]") {
    SECTION("using insert(key, value)") {
        sp_hash_table<key_type, value_type> t;
        t.insert("key1", 1);
        REQUIRE(t.size() == 1);
        auto found1 = t.find("key1");
        auto end1 = t.end();
        REQUIRE(found1 != end1);
        REQUIRE(found1->second == 1);
        t.insert("key2", 2);
        REQUIRE(t.size() == 2);
        auto found2 = t.find("key2");
        auto end2 = t.end();
        REQUIRE(found2 != end2);
        REQUIRE(found2->second == 2);
        t.insert("key3", 3);
        REQUIRE(t.size() == 3);
        auto found3 = t.find("key3");
        auto end3 = t.end();
        REQUIRE(found3 != end3);
        REQUIRE(found3->second == 3);
        t.insert("key4", 4);
        REQUIRE(t.size() == 4);
        auto found4 = t.find("key4");
        auto end4 = t.end();
        REQUIRE(found4 != end4);
        REQUIRE(found4->second == 4);
        t.insert("key5", 5);
        REQUIRE(t.size() == 5);
        auto found5 = t.find("key5");
        auto end5 = t.end();
        REQUIRE(found5 != end5);
        REQUIRE(found5->second == 5);
        t.insert("key6", 6);
        REQUIRE(t.size() == 6);
        auto found6 = t.find("key6");
        auto end6 = t.end();
        REQUIRE(found6 != end6);
        REQUIRE(found6->second == 6);
    }
    SECTION("using insert(Item)") {
        sp_hash_table<key_type, value_type> t;
        t.insert("key1", 1);
        t.insert("key2", 2);
        t.insert("key3", 3);
        t.insert("key4", 4);
        t.insert("key5", 5);
        t.insert("key6", 6);
        REQUIRE(t.size() == 6);
        REQUIRE(t.find("key1") != t.end());
        REQUIRE(t.find("key2") != t.end());
        REQUIRE(t.find("key3") != t.end());
        REQUIRE(t.find("key4") != t.end());
        REQUIRE(t.find("key5") != t.end());
        REQUIRE(t.find("key6") != t.end());
        REQUIRE(t.find("key1")->first == "key1");
        REQUIRE(t.find("key2")->first == "key2");
        REQUIRE(t.find("key3")->first == "key3");
        REQUIRE(t.find("key4")->first == "key4");
        REQUIRE(t.find("key5")->first == "key5");
        REQUIRE(t.find("key6")->first == "key6");
    }
    SECTION("when inserting an element with a key already in table") {
        sp_hash_table<key_type, value_type> t;
        t.insert("key1", 1);
        t.insert("key2", 2);
        t.insert("key2", 42);
        REQUIRE(t["key2"] == 42);
    }
}

TEST_CASE("Erasure of items works correctly", "[sp_hash_table]")  {
    sp_hash_table<key_type, value_type> t;
    t.insert("key1", 1);
    t.insert("key2", 2);
    t.insert("key3", 3);
    t.insert("key4", 4);
    t.insert("key5", 5);
    t.insert("key6", 6);
    REQUIRE(t.size() == 6);
    SECTION("when the key given exists in the table")  {
        t.erase("key1");
        REQUIRE(t.size() == 5);
        REQUIRE(t.find("key1") == t.end());
        t.erase("key2");
        REQUIRE(t.size() == 4);
        REQUIRE(t.find("key2") == t.end());
        t.erase("key3");
        REQUIRE(t.size() == 3);
        REQUIRE(t.find("key3") == t.end());
        t.erase("key4");
        REQUIRE(t.size() == 2);
        REQUIRE(t.find("key4") == t.end());
        t.erase("key5");
        REQUIRE(t.size() == 1);
        REQUIRE(t.find("key5") == t.end());
        t.erase("key6");
        REQUIRE(t.size() == 0);
        REQUIRE(t.find("key6") == t.end());
    }
    SECTION("when the key given does not exist in table") {
        t.erase("non-existent");
        REQUIRE(t.size() == 6);
    }
}

TEST_CASE("Iterators function correctly", "[sp_hash_table]") {
    sp_hash_table<key_type, value_type> t;
    sp_hash_table<key_type, value_type> empty;
    t.insert("key1", 1);
    t.insert("key2", 2);
    t.insert("key3", 3);
    t.insert("key4", 4);
    t.insert("key5", 5);
    t.insert("key6", 6);
    SECTION("plain iterators") {
        sp_hash_table<key_type, value_type>::iterator it1 = t.begin();
        ++++++++it1;
        REQUIRE(it1++ != t.end());
        REQUIRE(++it1 == t.end());
        sp_hash_table<key_type, value_type>::iterator it2 = empty.begin();
        REQUIRE(it2 == empty.end());
    }
    SECTION("const_iterators") {
        const sp_hash_table<key_type, value_type>& ref1 = t;
        const sp_hash_table<key_type, value_type>& ref2 = empty;
        sp_hash_table<key_type, value_type>::const_iterator it1 = ref1.begin();
        ++++++++it1;
        REQUIRE(it1++ != ref1.end());
        REQUIRE(++it1 == ref1.end());
        sp_hash_table<key_type, value_type>::const_iterator it2 = ref2.begin();
        REQUIRE(it2 == ref2.end());
    }
}

TEST_CASE("Access operator functions correctly", "[sp_hash_table]") {
    sp_hash_table<key_type, value_type> t;
    sp_hash_table<key_type, value_type> empty;
    t.insert("key1", 1);
    t.insert("key2", 2);
    t.insert("key3", 3);
    t.insert("key4", 4);
    t.insert("key5", 5);
    t.insert("key6", 6);
    SECTION("when the key given exists in table") {
        REQUIRE(t["key1"] == 1);
        REQUIRE(t["key2"] == 2);
        REQUIRE(t["key3"] == 3);
        REQUIRE(t["key4"] == 4);
        REQUIRE(t["key5"] == 5);
        REQUIRE(t["key6"] == 6);
    }
    SECTION("when the key given does not exist in table") {
        // it is added with a default value, whatever it is
        REQUIRE(!t.contains("non-existent"));
        value_type a = t["non-existent"];
        REQUIRE(t.contains("non-existent"));
        REQUIRE(t.size() == 7);
    }
}




