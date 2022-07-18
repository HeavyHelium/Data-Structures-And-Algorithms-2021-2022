#include <iostream>
#include <algorithm>


template<typename T>
struct avl_tree {
    using KeyType = T;

    avl_tree() = default;

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

    void insert(const KeyType& key) {
        insert_rec(key); // insert and update node heights
    }

    ~avl_tree() {
        clear();
    }
private:
    struct node {
        KeyType key;
        int height = 0; // Data Structure Augmentation
        node* left = nullptr;
        node* right = nullptr;
    };

    ///@brief so as to follow the mathematical def of tree height
    static int node_height(const node* node) {
        if(node) {
            return node->height;
        }
        return -1;
    }


    static int delta(const node* root) {
        int d = 0;
        if(root) {
            d = std::abs(node_height(root->left) -
                         node_height(root->right));
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
        root->height = 1 + std::max(node_height(root->right),
                                    node_height(root->left));
        int diff = delta(root);



    }

    static void right_rotate(node*& root) {
        node* new_root = root->left;
        node* temp = root;

        temp->left = new_root->right;
        root = new_root;
        root->right = temp;
    }

    static void left_rotate(node*& root) {
        node* new_root = root->right;
        node* temp = root;

        temp->right = new_root->left;
        root = new_root;
        root->left = temp;
    }


    node* root = nullptr;
    int m_size = 0;
};

/// require heights of left & right node to differ by at most 1


int main() {
    std::cout << "Hello world" << std::endl;

    return 0;
}
