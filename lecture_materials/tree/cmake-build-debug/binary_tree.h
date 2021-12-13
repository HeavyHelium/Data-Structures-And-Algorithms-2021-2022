#ifndef TREE_BINARY_TREE_H
#define TREE_BINARY_TREE_H

#include <iostream>
#include <cstddef>
#include <algorithm>
#include <queue>

template<typename T>
class tree
{
public:
    tree() = default;
    ~tree() { clear(root); }
    tree(const tree& other)
        : root(copy(other.root)),
          m_size(other.m_size)
    {}
    tree(tree&& other)
    {
        std::swap(root, other.root);
        std::swap(m_size, other.m_size);
    }
    tree& operator=(tree other)
    {
        std::swap(root, other.root);
        std::swap(m_size, other.m_size);
        return *this;
    }

    bool find(const T& key) const
    { return find_element(root, key); }
    bool insert(const T& key, int path[])
    {
        if(insert_element(root, key, path))
            {++m_size; return true;}
        return false;
    }
    bool erase(const T& key)
    {
        if(erase_element(root, key))
            { --m_size; return true; }
        return false;
    }

    std::size_t size()   const { return m_size; }
    std::size_t height() const { return height(root); }

    template<typename Function>
    void map(const Function& fun) { map(fun, root); }

    void print() const { print_by_levels(root); }

    void print_by_levels() const { printByLevels(root); }
private:
    struct node
    {
        T key;
        node* child;
        node* brother;
        node(const T& key, node* child = nullptr, node* brother = nullptr)
            : key(key),
              child(child),
              brother(brother)
        {}
    };

    static void clear(node* root)
    {
        if(root)
        {
            clear(root->child);
            clear(root->brother);
            delete root;
        }
    }

    static node* copy(node* root)
    {
        return root ?
               new node(root->key, copy(root->child), copy(root->brother)) :
               root;
    }

    static bool find_element(node* root, const T& key)
    {
        if(!root) return false;
        if(root->key == key) return true;
        return find_element(root->brother) ||
                find_element(root->child);
    }

    static std::size_t height(node* root)
    {
        if(!root) return 0;
        return std::max(1 + height(root->child),
                        height(root->brother));
    }

    template <typename Function>
    void map(const Function& fun, node* root)
    {
        if(!root) return;
        fun(root->key);
        map(root->brother);
        map(root->child);
    }

    static bool erase_element(node*& root, const T& key)
    {
        if(!root) return false;
        if(root->key == key)
        {
            node* to_delete = root;
            if(!root->child)
                root = root->brother;
            else if(!root->brother)
                root = root->child;
            else
            {
                node* it = root->child;
                /// we reach the last brother
                while(it->brother) it = it->brother;
                /// we make its brother the child of root's brother
                it->brother = root->brother->child;
                /// we make the child of root's brother to be root->child
                root->brother->child = root->child;
                /// then we set root to be its brother
                root = root->brother;
            }
            delete to_delete;
            return true;
        }
        else return erase_element(root->brother) ||
                    erase_element(root->child);
    }

    static bool insert_element(node*& root,
                               const T& key,
                               const int* path)
    {
        /// Attaches as a left brother of root
        if(path[0] < 0)
        {
            root = new node(key, nullptr, root);
            return true;
        }
        /// now we must attach as a child
        /// no child -> wrong path?
        if(!root) return false;
        /// go down to children
        if(path[0] == 0)
            return insert_element(root->child, key, path + 1);
        /// find a suitable node in the fraternity
        node* it = root;
        int pos = path[0];
        while(it->brother && pos > 0)
            { it = it->brother; --pos; }
        return it->brother?
               insert_element(it->brother->child, key, path + 1) :
               false;
    }
    static void print_by_levels(const node* root)
    {
        if(!root) return;
        std::queue<const node*> q;
        q.push(root);
        for(;;)
        {
            const node* current = q.front();
            q.pop();
            if(!current)
            {
                std::cout << '\n';
                if(q.empty()) return;
            }
            else
            {
                // add brothers of child
                std::cout << current->key << ' ';
                for(const node* it = current->child; it; it = it->brother)
                    q.push(it);
                q.push(nullptr);
            }
        }
    }

    node* root = nullptr;
    std::size_t m_size = 0;
};

#endif //TREE_BINARY_TREE_H
