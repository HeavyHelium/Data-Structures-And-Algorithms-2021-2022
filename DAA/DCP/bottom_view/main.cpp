#include <iostream>
#include <algorithm>
#include <utility>
#include <map>
#include <queue>
#include <vector>
#include <cassert>

struct tree {
    struct node {
        int key = -1;
        int head_dist = -1; // augmenting the structure to fill these values
        node* left = nullptr;
        node* right = nullptr;
    };
    tree() {
        create_test_tree();
        fill_hd_rec(root);
    }
    tree(std::initializer_list<int> elems) {
        for(int elem : elems) {
            insert(elem);
        }
        fill_hd_rec(root);
    }
    tree(const tree& other) = delete;
    tree& operator=(const tree& other) = delete;

    void insert(const int elem) {
        insert_rec(root, elem);
    }

    /// returns a map so as to save the bottom most nodes
    /// for each horizontal level
    std::map<int, int> bottom_view() const {
        std::map<int, int> views_levels;
        if(root) {
            /// in a breadth-fist search manner traverse the tree
            std::queue<const node*> q;
            q.push(root);
            while(!q.empty()) {
                const node* current = q.front();
                q.pop();
                if(current->left) {
                    q.push(current->left);
                }
                if(current->right) {
                    q.push(current->right);
                }
                views_levels[current->head_dist] = current->key;
            }
        }
        return std::move(views_levels); // NRVO is not guaranteed
    }

    ~tree() {
        clear(root);
    }

private:
    void create_test_tree() {
        root = new node{ 20 };
        root->left = new node{ 8 };
        root->right = new node{ 22 };
        root->left->left = new node{ 5 };
        root->left->right = new node{ 3 };
        root->right->left = new node{ 4 };
        root->right->right = new node{ 25 };
        root->left->right->left = new node{ 10 };
        root->left->right->right = new node{ 14 };
    }
    static void fill_hd_rec(node*& root, int curr_val = 0) {
        if(root) {
            root->head_dist = curr_val;
            fill_hd_rec(root->left, curr_val - 1);
            fill_hd_rec(root->right, curr_val + 1);
        }
    }
    static void insert_rec(node*& root, const int elem) {
        if(!root) {
            root = new node { elem };
        } else if(root->key < elem) {
            insert_rec(root->right, elem);
        } else {
            insert_rec(root->left, elem);
        }
    }
    static void clear(node*& root) {
        if(root) {
            clear(root->left);
            clear(root->right);
        }
        delete root;
        root = nullptr;
    }
    static void bottom_view_rec(node* root, std::map<int, int>& t) {
        if(root) {
            bottom_view_rec(root->left, t);

        }
    }
    node* root = nullptr;
};


int main() {
    tree t;
    auto m = t.bottom_view();
    std::vector<int> res{ 5, 10, 4, 14, 25 };
    int i = 0;
    for(auto pair : m) {
        assert(pair.second == res[i++]);
    }
    return 0;
}
