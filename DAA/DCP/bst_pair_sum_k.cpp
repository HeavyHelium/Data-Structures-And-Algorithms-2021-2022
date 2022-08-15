#include <iostream>
#include <vector>
#include <utility>
#include <initializer_list>
#include <unordered_set>
#include <climits>

struct bst {
    struct node {
        int key;
        node* left = nullptr;
        node* right = nullptr;
    };

    bst(std::initializer_list<int> lst) {
        for(int elem : lst) {
            insert(elem);
        }
    }

    bst(const bst& other) = delete;
    void operator=(const bst& other) = delete;

    void insert(int key) {
        insert_rec(root, key);
    }

    std::vector<int> inorder() const {
        std::vector<int> path;
        inorder_rec(root, path);
        return std::move(path);
    }

    std::pair<int, int> sum_of_k(int k) const {
        std::vector<int> path = inorder();
        std::unordered_set<int> hash;
        for(int e : path) {
            hash.insert(e);
        }
        for(int e : path) {
            if(hash.find(k - e) != hash.end()) {
                return { e, k - e };
            }
        }
        return { INT_MAX, INT_MAX };
    }

    ~bst() {
        free_rec(root);
    }
private:
    static void free_rec(node* root) {
        if(!root) {
            return;
        }
        free_rec(root->left);
        free_rec(root->right);
        delete root;
    }
    static void insert_rec(node*& root, int key) {
        if(!root) {
            root = new node{ key };
        } else if(root->key < key) {
            insert_rec(root->right, key);
        } else {
            insert_rec(root->left, key);
        }
    }
    static void inorder_rec(const node* root, std::vector<int>& path) {
        if(!root) {
            return;
        } else {
            inorder_rec(root->left, path);
            path.push_back(root->key);
            inorder_rec(root->right, path);
        }
    }

    node* root = nullptr;
};

int main() {
    bst t{ 10, 5, 15, 11, 15 };

    auto res = t.sum_of_k(26);
    if(res.first != INT_MAX) {
        std::cout << res.first << ", "
                  << res.second
                  << std::endl;
    } else {
        std::cout << "No such pair of nodes in tree."
                  << std::endl;
    }

    return 0;
}
