#ifndef LECTURE_06_12_BINARY_SEARCH_TREE_H
#define LECTURE_06_12_BINARY_SEARCH_TREE_H
/// Binary trees, binary search trees
#include <algorithm>
#include <cstddef>
#include <initializer_list>

template<typename T>
class binary_search_tree
{
public:
    binary_search_tree()  {}
    binary_search_tree(const std::initializer_list<T>& lst)
    { for(const T& elem : lst) insert(elem); }
    ~binary_search_tree() { clear(root); }
    binary_search_tree(const binary_search_tree& other)
        : root(copy(other.root)), m_size(other.m_size)
    {}
    binary_search_tree(binary_search_tree&& other)
    {
        std::swap(root, other.root);
        std::swap(m_size, other.m_size);
    }
    binary_search_tree& operator=(binary_search_tree other)
    {
        std::swap(root, other.root);
        std::swap(m_size, other.m_size);
        return *this;
    }
    bool find  (const T& key) const { return find_element(root, key); }
    void insert(const T& key)       { insert_element(root, key); ++m_size; }
    void erase (const T& key)       { erase_element(root, key); /*--m_size;*/ }
    std::size_t size()        const { return m_size; }
    std::size_t height()      const { return calculate_height(root); }
    bool balanced()           const { return is_balanced(root); }
    bool perfectly_balanced() const { return is_perfectly_balanced(root); }
    void print()              const { print_infix(root); std::cout << std::endl; }
    /// mind that this does a dfs on the tree and hence is O(n);
    /// size(), on the other hand, is O(1)
    std::size_t weight()      const { return weight(root); }
private:
    struct node
    {
        T key;
        node* left;
        node* right;
        node(const T& key,
             node* left = nullptr,
             node* right = nullptr)
             : key(key), left(left), right(right)
        {}
    };
    node* root = nullptr;
    std::size_t m_size = 0;
    static void clear(node* root)
    {
        if(root)
        {
            clear(root->left);
            clear(root->right);
            delete root;
        }
    }
    static node* copy(node* root)
    {
        return root?
        new node(root->key, root->left, root->right):
                                                root;
    }
    static bool find_element(node* root, const T& key)
    {
        if(!root) return false;
        if(root -> key == key) return true;
        return key < root -> key ?
               find_element(root -> left, key) :
               find_element(root -> right, key);
    }
    static bool find_element_iter(node* root, const T& key)
   {
        node* current = root;
        while(current)
        {
            if(key == current->key) return true;
            (key > current->key) ? current = current->right : current = current->left;
        }
        return false;
   }
    /// we always insert an element as a leaf of the tree
    static void insert_element(node*& root, const T& key)
   {
        if(!root) root = new node(key);
        else if(key < root->key) insert_element(root->left, key);
        else insert_element(root->right, key);
   }
    void insert_element_iter(const T& key)
   {
        if(!root) root = new node(key);
        else
        {
            node* parent = root;
            node* next = key < (parent -> key) ?
                         parent -> left :
                         parent -> right;
            while(next)
            {
                if(key < (next -> key))
                    next = next -> left;
                else next = next -> right;
                parent = next;
            }
        // we have reached a nullptr and its parent is 'parent'
            if(key < parent -> key)
                parent -> left = new node(key);
            else parent -> right = new node(key);
        }
   }
    void erase_element(node*& root, const T& key)
   {
        if(!root) return; // tree is empty
        if(key == root -> key)
        {
            node* to_delete = root;
            if(!(root -> left))
                root = root -> right;
            else if (!(root -> right))
                root = root -> left;
            else
            {
                node* min_right = extract_min(root -> right);
                min_right -> left = root -> left;
                min_right -> right = root -> right;
                root = min_right;
            }
            delete to_delete;
            --m_size;
        }
        else if(key > root -> key)
            erase_element(root -> right, key);
        else erase_element(root -> left, key);
   }
    static node* extract_min(node*& root)
   {
        if(root -> left)
            return extract_min(root -> left);
        else
        {
            node* temp = root;
            root = root -> right;
            return temp;
        }
   }
    static std::size_t calculate_height(const node* root)
   {
        if(!root) return 0;
        return 1 + std::max(calculate_height(root -> left),
                            calculate_height(root -> right));
   }
   /// not a particularly efficient way to check if a bst is balanced
   static bool is_balanced(const node* root)
   {
        if(!root) return true;
        if(std::labs((long)calculate_height(root -> right) - calculate_height(root -> left)) >= 2)
            return false;
        return is_balanced(root -> left) && is_balanced(root -> right);
   }
   /// the number of nodes in the tree
   static std::size_t weight(const node* root)
   {
        if(!root) return 0;
        return 1 + weight(root -> left) + weight(root -> right);
   }
   static bool is_perfectly_balanced(const node* root)
   {
        if(!root) return true;
        return labs((long)weight(root -> left) - weight(root -> right)) < 2 &&
                is_perfectly_balanced(root -> left) &&
                is_perfectly_balanced(root -> right);
   }
   static void print_infix(const node* root)
   {
        if(root)
        {
            print_infix(root -> left);
            std::cout << root -> key << " ";
            print_infix(root -> right);
        }
   }
};
#endif
