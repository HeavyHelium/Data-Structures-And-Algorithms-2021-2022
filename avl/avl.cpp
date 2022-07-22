#include <iostream>
#include <cassert>
#include <algorithm>
#include <utility>
#include <vector>
#include <initializer_list>

template<typename T>
struct avl_tree {
    using KeyType = T;

    struct node {
        KeyType key;
        int height = 0; // Data Structure Augmentation
        node* left = nullptr;
        node* right = nullptr;
    };

    avl_tree() = default;

    avl_tree(std::initializer_list<KeyType> lst) {
        for(const KeyType& item : lst) {
            insert(item);
        }
    }

    avl_tree(const avl_tree& other)
        : root(copy_rec(other.root)),
          m_size(other.m_size) {
    }

    avl_tree& operator=(const avl_tree& other) {
        if(this != &other) {
            node* temp = copy_rec(other.root);
            clear();
            root = temp;
            m_size = other.m_size;
        }
        return *this;
    }

    avl_tree& operator=(avl_tree&& other) {
        std::swap(root, other.root);
        std::swap(m_size, other.m_size);
        return *this;
    }

    avl_tree(avl_tree&& other) {
        std::swap(root, other.root);
        std::swap(m_size, other.m_size);
    }

    void clear() {
        clear_rec(root);
        m_size = 0;
    }
    ///@brief follows the mathematical definition of tree height
    int height() const {
        return node_height(root);
    }

    ///@returns the weight a.k.a. number of nodes currently in BST
    int size() const {
        return m_size;
    }

    bool empty() const {
        return !root;
    }

    void insert(const KeyType& key) {
        if(!contains(key)) {
            insert_rec(root, key); // insert and update node heights
            ++m_size;
        }
    }

    void erase(const KeyType& key) {
        erase_rec(root, key);
    }

    ///@returns nullptr if key is nowhere to be found in tree
    /// otherwise, returns a const pointer to node with the given key
    const node* find(const KeyType& key) const {
        return find_rec(root, key);
    }

    bool contains(const KeyType& key) const {
        return find(key);
    }


    std::vector<KeyType> inorder() const {
        std::vector<KeyType> path;
        inorder_rec(root, path);
        return std::move(path);
    }

    void swap(avl_tree& other) {
        std::swap(root, other->root);
        std::swap(m_size, other->m_size);
    }

    ~avl_tree() {
        clear();
    }
private:
    static node*& find_max(node*& root) {
        if(!root || !root->right) {
            return root;
        } else {
            return find_max(root->right);
        }
    }
    static node*& find_min(node*& root) {
        if(!root || !root->left) {
            return root;
        } else {
            return find_min(root->left);
        }
    }
    static void update_rec(node*& root) {
        if(root) {
            root->height = 1 + std::max(node_height(root->right),  // update current node height
                                        node_height(root->left));
            int diff = delta(root);

            if(diff < -1) { // it is right-heavy
                int temp = delta(root->right);
                if(temp <= 0) {  // it is doubly right-heavy or right child is balanced
                    left_rotate(root);
                } else {
                    node* old_root = root;
                    right_rotate(root->right);
                    left_rotate(root);
                }
            } else if(diff > 1) {
                int temp = delta(root->left);
                if(temp >= 0) {  // it is doubly left-heavy or left child is balanced
                    right_rotate(root);
                } else {
                    node* old_root = root;
                    left_rotate(root->left);
                    right_rotate(root);
                }
            }
        }

    }
    static const node* find_rec(const node* root, const KeyType& key) {
        if(!root || root->key == key) {
            return root;
        } else if(root->key < key) {
            return find_rec(root->right, key);
        } else {
            return find_rec(root->left, key);
        }
    }

    static void inorder_rec(node* root, std::vector<KeyType>& path) {
        if(root) {
            inorder_rec(root->left, path);
            path.push_back(root->key);
            inorder_rec(root->right, path);
        }
    }

    ///@brief so as to follow the mathematical definition of tree height
    static int node_height(const node* node) {
        if(node) {
            return node->height;
        }
        return -1;
    }

    ///@returns the balance "factor" of a given node
    static int delta(const node* root) {
        int d = 0;
        if(root) {
            d = node_height(root->left) -
                node_height(root->right);
        }
        return d;
    }

    static void clear_rec(node*& root) {
        if(root) {
            clear_rec(root->left);
            clear_rec(root->right);
            delete root;
            root = nullptr;
        }
    }

    static node* copy_rec(const node* other_root) {
        node* root = nullptr;
        if(other_root) {
            root = new node{ other_root->key,
                             other_root->height,
                             copy_rec(other_root->left),
                             copy_rec(other_root->right) };
        }
        return root;
    }

    static void insert_rec(node*& root, const KeyType& key) {
        if(!root) {
            root = new node{ key };
        } else if(root->key < key) {
            insert_rec(root->right, key);
        } else {
            insert_rec(root->left, key);
        }
        update_rec(root);
    }

    static void erase_rec(node*& root, const KeyType& key) {
        if(root) {
            if(root->key == key) {
                node* to_delete = nullptr;
                if(!root->left && !root->right) { // single leaf
                    to_delete = root;
                    root = nullptr;
                } else if(root->left && !root->right) { // left child only
                    to_delete = root;
                    root = root->left;
                } else if(!root->left && root->right) { // right child only
                    to_delete = root;
                    root = root->right;
                } else { // two children
                    node*& predecessor = find_max(root->left);
                    root->key = predecessor->key;
                    to_delete = predecessor;
                    predecessor = predecessor->left;
                }
                delete to_delete;
            } else if(root->key < key) {
                erase_rec(root->right, key);
            } else {
                erase_rec(root->left, key);
            }

            update_rec(root);

        } else {
            throw std::runtime_error("No such element in tree.");
        }
    }

    static void right_rotate(node*& root) {
        node* new_root = root->left;
        node* temp = root;

        temp->left = new_root->right;
        temp->height = node_height(temp); // update height of old root

        root = new_root;
        root->right = temp;
        root->height = node_height(root); // update height of new root
    }
    /// symmetrical to right_rotate function
    static void left_rotate(node*& root) {
        node* new_root = root->right;
        node* temp = root;

        temp->right = new_root->left;
        temp->height = node_height(temp);

        root = new_root;
        root->left = temp;
        root->height = node_height(root);
    }


    node* root = nullptr;
    int m_size = 0;
};


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
