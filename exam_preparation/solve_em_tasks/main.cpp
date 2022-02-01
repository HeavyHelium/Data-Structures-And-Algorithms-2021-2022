#include <iostream>
#include <initializer_list>
#include <vector>

class bst {
    struct node {
        int data = 0;
        node* left_child = nullptr;
        node* right_child = nullptr;
    };
    node* root = nullptr;
    int m_size = 0;
public:
    bst() = default;
    bst(const std::initializer_list<int>& lst) {
        for(int elem : lst) {
            insert(elem);
        }
    }
    bst(const bst& other)
            : root(copy_tree(other.root)),
              m_size(other.m_size) {}
    bst(bst&& other) noexcept {
        std::swap(root, other.root);
        std::swap(m_size, other.m_size);
    }
    bst& operator=(bst other) {
        std::swap(other.root, root);
        std::swap(m_size, other.m_size);
        return *this;
    }
    void insert(int elem) {
        insert_elem(root, elem);
        ++m_size;
    }
    void erase(int elem) {
        if(erase_elem(root, elem)) {
            --m_size;
        } else {
            throw std::runtime_error("element was not in tree");
        }
    }
    node* find(int elem) {
        return find_rec(root, elem);
    }
    int size() const {
        return m_size;
    }
    bool empty() const {
        return !root;
    }
    void print() const {
        std::vector<int> path = inorder();
        for(int i : path) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
    std::vector<int> inorder() const {
        std::vector<int> path;
        inorder(root, path);
        return path;
    }
    bool balanced() const {
        bool b = true;
        balanced_helper(root, b);
        return b;
    }
    ~bst() {
        free_tree(root);
    }
private:
    static void free_tree(node* root) {
        if(!root) {
            return;
        }
        free_tree(root->left_child);
        free_tree(root->right_child);
        delete root;
    }
    static node* copy_tree(const node* other_root) {
        if(!other_root) {
            return nullptr;
        }
        return new node { other_root->data,
                          copy_tree(other_root->left_child),
                          copy_tree(other_root->right_child) };
    }
    // if we find that element, nothing is done
    static void insert_elem(node*& root, int elem) {
        if(!root) {
            root = new node { elem };
        } else if(elem > root->data) {
            insert_elem(root->right_child, elem);
        } else if(elem < root->data) {
            insert_elem(root->left_child, elem);
        }
    }
    static bool erase_elem(node*& root, int elem) {
        if(!root) return false;
        if(root->data < elem) {
            return erase_elem(root->right_child, elem);
        } else if(root->data > elem) {
            return erase_elem(root->left_child, elem);
        } else {
            /// if it's got one of his children missing
            node* to_delete = root;
            if(!root->right_child) {
                root = root->left_child;
            } else if(root->right_child) {
                root = root->right_child;
            } else {
                node* min_right = extract_min(root->right_child);
                root = min_right;
            }
            delete to_delete;
            return true;
        }
    }
    /// detach the node and fix the place it's been detached
    static node* extract_min(node*& root) {
        if(!root || !root->left_child) {
            return root;
        }
        node* min = root;
        root = root->right_child;
        return root;
    }
    static node* find_rec(node* root, int elem) {
        if (!root) {
            return root;
        }
        if (root->data == elem) {
            return root;
        }
        node* l = find_rec(root->left_child, elem);
        return l ? l : find_rec(root->right_child, elem);
    }
    static void inorder(const node* root, std::vector<int>& path) {
        if(!root) {
            return;
        }
        inorder(root->left_child, path);
        path.push_back(root->data);
        inorder(root->right_child, path);
    }
    static int exactly_n_helper(const node* root, int n, int& accum) {
        if(!root) {
            return 0;
        }
        int w1 = exactly_n_helper(root->left_child, n, accum);
        int w2 = exactly_n_helper(root->right_child, n, accum);
        int current_weight = w1 + w2 + 1;
        if(current_weight == n) {
            ++accum;
        }
        return current_weight;
    }
    static void erase_even(node*& root) {
        if(!root) {
            return;
        }
        erase_even(root->left_child);
        erase_even(root->right_child);
        if(root->data % 2 == 0) {
            erase_elem(root, root->data); // this is O(1), no worries
        }
    }
    static int height(const node* root) {
        if(!root) {
            return 0;
        }
        return 1 + std::max(height(root->right_child),
                            height(root->left_child));
    }
    static int weight(const node* root) {
        if(!root) {
            return 0;
        }
        return 1 +
               height(root->right_child) +
               height(root->left_child);
    }
    /// saves state in a parameter, returns heights;
    static int balanced_helper(const node* root, bool& balanced) {
        if(!root) {
            return 0;
        }
        int height1 = balanced_helper(root->left_child, balanced);
        if(!balanced) {
            balanced = false;
            return -1;
        }
        int height2 = balanced_helper(root->right_child, balanced);
        if(abs(height1 - height2) > 2 || !balanced) {
            balanced = false;
            return -1;
        }
        return 1 + std::max(height1, height2);
    }
};

int main() {

    return 0;
}