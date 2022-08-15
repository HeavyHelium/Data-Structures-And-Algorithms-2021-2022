#include <iostream>
#include <cassert>
#include <string>
#include <stdexcept>

class binary_tree {
    struct node {
        int data{ 0 };
        node* l{ nullptr };
        node* r{ nullptr };
    };
    node* root{ nullptr };
    int m_size{ 0 };
public:
    binary_tree(const std::string& line) {
        const char* data = line.c_str();
        try { m_size = read_tree_helper(root, data); }
        catch(...) {
            free_that_tree(root);
            throw;
        }
    }
    binary_tree(const binary_tree& other) {
        try {
            root = copy_that_tree(other.root);
            m_size = other.m_size;
        }
        catch(...) {
            free_that_tree(root);
            throw;
        }
        m_size = other.m_size;
    }
    binary_tree(binary_tree&& other) {
        std::swap(root, other.root);
        std::swap(m_size, other.m_size);
    }
    /// @brief copy-and-swap idiom
    binary_tree& operator=(binary_tree other) {
        std::swap(root, other.root);
        std::swap(m_size, other.m_size);
        return *this;
    }
    ~binary_tree() {
        free_that_tree(root);
    }
    int get_unival_subtree_cnt() const {
        int sofar = 0;
        get_unival_subtree_cnt_helper(root, sofar);
        return sofar;
    }
private:
    /**@brief check if left subtree is uval
     check if right subtree is uval
     see if left and right values have the same value*/
    static int get_unival_subtree_cnt_helper(const node* root, int& so_far) {
        if(!root) {
            return 0;
        }
        if(!root->l &&
           !root->r) {
           ++so_far;
           return root->data;
        } else if(!root->l) {
            int val = get_unival_subtree_cnt_helper(root->r,
                                                    so_far);
            if(val == root->data) {
                ++so_far;
                return val;
            } else {
                return -1;
            }
        } else if(!root->r) {
            int val = get_unival_subtree_cnt_helper(root->l,
                                                    so_far);
            if(val == root->data) {
                ++so_far;
                return val;
            } else {
                return -1;
            }
        } else {
            int val_l = get_unival_subtree_cnt_helper(root->l,
                                                      so_far);
            int val_r = get_unival_subtree_cnt_helper(root->r,
                                                      so_far);
            if(val_l == -1 ||
               val_r == -1 ||
               val_r != root->data ||
               val_l != root->data ||
               val_l != val_r) {
                return -1;
            } else {
                ++so_far;
                return val_l; // or val_r or root->data, all the same :)
            }
        }

    }
    static int read_tree_helper(node*& root, const char*& nodes) {
        int val = extract_int(nodes);
        if(val == -1) {
            return 0;
        }
        root = new node{ val };
        int cnt1 = read_tree_helper(root->l, nodes);
        int cnt2 = read_tree_helper(root->r, nodes);
        return cnt1 + cnt2 + 1;
    }
    static void skip_whitespace(const char*& line) {
        while(*line && is_whitespace(*line)) {
            ++line;
        }
    }
    static bool isdigit(char ch) {
        return ch <= '9' &&
               ch >= '0';
    }
    static bool is_whitespace(char ch) {
        return ch == ' ' ||
               ch == '\n' ||
               ch == '\t';
    }
    static bool issign(char ch) {
        return ch == '-';
    }
    static int extract_int(const char*& line){
        skip_whitespace(line);
        int sign = 1;
        if(!*line && !isdigit(*line) && !issign(*line)) {
                throw std::runtime_error("error in reading data");
        } else {
            if(issign(*line)) {
                sign = -1;
                ++line;
            }
            int number = 0;
            while(isdigit(*line)) {
                number *= 10;
                number += (*line++ - '0');
            }
            skip_whitespace(line);
            return number * sign;
        }
    }
    static void free_that_tree(node* root) {
        if(!root) {
            return;
        } else {
            free_that_tree(root->l);
            free_that_tree(root->r);
            delete root;
        }
    }
    static node* copy_that_tree(const node* root) {
        if(!root) {
            return nullptr;
        } else {
            node* left = copy_that_tree(root->l);
            node* right = copy_that_tree(root->r);
            return new node{ root->data, left, right };
        }
    }

};

int main() try {
    std::string data1{ "0 1 -1 -1 0 1 1 -1 -1 1 -1 -1 0 -1 -1" };
    std::string data2{ "1 1 -1 -1 1 1 1 -1 -1 1 -1 -1 1 -1 -1" };
    std::string data3{ "5 1 5 -1 -1 5 -1 -1 5 -1 5 -1 -1" };
    binary_tree tree(data3);
    std::cout << tree.get_unival_subtree_cnt() << std::endl;
    return 0;
} catch(const std::exception& e) {
    std::cerr << "something went terribly wrong: " <<
                 e.what() <<
                 std::endl;
    return -1;
}
