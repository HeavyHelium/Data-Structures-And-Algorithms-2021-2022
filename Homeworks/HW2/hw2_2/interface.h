#pragma once
#include <string>
#include <list>
#include "data_parser.h"

using std::string;

class Hierarchy
{
public:
    Hierarchy(Hierarchy&& r) noexcept;
    Hierarchy(const Hierarchy& r);
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
    int num_all_subordinates(const string& name) const;
    Hierarchy join(const Hierarchy& right) const;
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
    static std::size_t weight(Node* root);
    static std::size_t height(Node* root);
    Node* find_by_key(const std::string& key) const;
    parent_child find_parent(const std::string& child) const;
    parent_child find_parent(const std::string& child);
    static void free(Node* root);
    static Node* copy(const Node* root);
    static parent_child find_parent(Node* root, const std::string& child_value);
    static Node* find_rec(Node* root, const std::string& name);
    static Node* add_as_child(Node* parent, const std::string& name);
    static int num_overloaded_helper(Node* root, int N, int& overloaded);
    Node* root = nullptr;
    std::size_t m_size = 1;
};

