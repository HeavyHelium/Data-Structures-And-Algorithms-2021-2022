#ifndef TREE_BINARY_TREE_H
#define TREE_BINARY_TREE_H

#include <iostream>
#include <cstddef>
#include <algorithm>
#include <queue>

/// implements a tree using left-child, right-sibling nodes
template<typename T>
class tree
{
    friend class Hierarchy;
public:
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
    tree() = default;
    tree(const T& root_key)
        : root(new node(root_key)), m_size(1)
    {}
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
    int number_of_children(const T& key) const
    {
        assert(root && "empty tree");
        const node* temp = find_node(key);
        if(!temp) return -1;
        int cnt = 0;
        for(const node* it = temp->child; it; it = it->brother, ++cnt)
            ;
        return cnt;
    }
    const node* find_node(const T& key) const
    {
        assert(root && "empty tree");
        std::queue<const node*> q;
        q.push(root);
        for(;!q.empty();)
        {
            const node* current = q.front();
            q.pop();
            if(current->key == key)
            {
                // TODO: Refactor
                return current;
            }
            else
                for(const node* it = current->child; it; it = it->brother)
                    q.push(it);
        }
        /// haven't found it, no such node
        return nullptr;
    }

    node* find_node(const T& key)
    {
        return const_cast<node*>((const_cast<const tree<T>*>(this))->find_node(key));
    }

    void insert_as_child(const T& parent_key, const T& child_key)
    {
        node* parent = find_node(parent_key);
        if(!parent) throw std::logic_error("no such parent");
        if(parent)
        {
            parent->child = new node(child_key, nullptr, parent->child);
            ++m_size;
        }
        else throw std::logic_error("tree has no such ");
    }

    const node* find_parent(const T& key) const
    {
        assert(root && "empty tree");
        if(root->key == key) throw std::logic_error("root is not its own child");
        std::queue<const node*> q;
        q.push(root);
        for(;;)
        {
            const node* current = q.front();
            q.pop();
                // add brothers of child
            for(const node* it = current->child; it; it = it->brother)
            {
                if(it->key == key) return current;
                q.push(it);
            }
        }
        return nullptr;
    }

    node* find_parent(const T& key)
    {
        return const_cast<node*>((const_cast<const tree<T>*>(this))->find_parent(key));
    }

    const T& get_parent_key(const T& child) const
    {
        const node* temp = find_parent(child);
        if(!temp) throw std::logic_error("no child with that key");
        return temp->key;
    }
    /// @throws when:
    /// tree is empty
    bool find(const T& key) const
    { return find_element(root, key); }
    std::vector<int> find_path_to(const std::string& str) const
    {
        return std::vector<int>{};
        /*
        if(!root) throw std::logic_error("tree has no elements");
        if(root->key == str) return std::vector<int>{-1};
        std::vector<int> path;
        std::queue<const node*> q;
        q.push(root);
        q.push(nullptr);
        for(;;)
        {
            const node* current = q.front();
            q.pop();
            if(!current)
            {
                path.push_back(0);
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
        }*/

    }
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

    std::string print() const { return print_by_levels(root); }
private:

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
        return find_element(root->brother, key) ||
                find_element(root->child, key);
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
    /// bfs
    static std::string print_by_levels(const node* root)
    {
        if(!root) return "";
        std::string result;
        std::queue<const node*> q;
        q.push(root);
        for(;;)
        {
            if(q.empty()) return result;
            const node* current = q.front();
            q.pop();
            for(const node* it = current->child; it; it = it->brother)
            {
                result += current->key + "-" + it->key += "\n";
                q.push(it);
            }
        }
    }

    node* root = nullptr;
    std::size_t m_size = 0;
};

#endif //TREE_BINARY_TREE_H
