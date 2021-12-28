#pragma once
#include <string>
#include <list>
#include "data_parser.h"

using std::string;

class Hierarchy
{
    static inline const std::string boss_name = "Uspeshnia";
    class Node;
public:
    Hierarchy(Hierarchy&& r) noexcept;
    Hierarchy(const Hierarchy& r);
    Hierarchy(Node* root);
    Hierarchy(const string& data);
    ~Hierarchy() noexcept;
    void operator=(const Hierarchy&) = delete;

    string print() const;

    int longest_chain() const;
    bool find(const string& name) const;
    int num_employees() const;
    int num_overloaded(int level = 20) const;

    string manager(const string& name) const;
    int num_subordinates(const string& name) const;
    unsigned long getSalary(const string& who) const;

    bool fire(const string& who);
    bool hire(const string& who, const string& boss);

    void incorporate();
    void modernize();
    Hierarchy join(const Hierarchy& right) const;

    int num_all_subordinates(const string& name) const;
private:
    struct Node
    {
        std::string name;
        std::list<Node*> subordinates;
    };
    struct parent_child
    {
        Node* parent = nullptr;
        std::list<Node*>::iterator child;
    };
    struct parent_child_ptrs
    {
        const Node* parent = nullptr;
        const Node* child = nullptr;
    };
    struct node_level
    {
       const Node* parent = nullptr;
       const Node* node = nullptr;
       const int level = -1;
    };
    struct left_right
    {
        const Node* left_tree = nullptr;
        Node* current_node = nullptr;
        const Node* right_tree = nullptr;
        int level = -1;
    };

    Node* find_by_key(const std::string& key) const;
    parent_child find_parent(const std::string& child) const;
    parent_child find_parent(const std::string& child);

    static unsigned long salary(const Node* root);
    static std::size_t weight(const Node* root);
    static std::size_t height(Node* root);

    static const Node* find_rec(const Node* root, const std::string& name);
    static Node* find_rec(Node* root, const std::string& name);
    static parent_child find_parent(Node* root, const std::string& child_value);
    static Node* is_subordinate(const Node* root, const std::string& key);
    static node_level find_level(const Node* root, const std::string& key);

    static Node* add_as_child(Node* parent, const std::string& name);

    static int num_overloaded_helper(Node* root, int N, int& overloaded);

    static void incorporate_helper(Node* root);
    static bool check_for_conflict(const Node* node, const Node* other_tree);
    static void modernize_helper(Node* root, bool even = true);
    static Node* join_helper(const Node* root1, const Node* root2);

    static void free(Node* root);
    static Node* copy(const Node* root);
    void add_root(const std::string& name = boss_name);
    static Node* generate_root(const std::string& name = boss_name);

    Node* root = nullptr;
    int m_size = 0;
};

