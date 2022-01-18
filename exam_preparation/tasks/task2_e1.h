#ifndef TASKS_TASK2_E1_H
#define TASKS_TASK2_E1_H
#include <cassert>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>
#include <initializer_list>
class bst {
    struct Node {
        int value;
        Node* left = nullptr;
        Node* right = nullptr;
    };
    Node* root = nullptr;
    int m_size = 0;
public:
    bool insert(int val) {
        if(insert(root, val)) {
            ++m_size;
            return true;
        }
        return false;
    }
    bool erase(int val) {
        if(erase(root, val)) {
            --m_size;
            return true;
        }
        return false;
    }
    int exactly_n(int n) {
        int acc = 0;
        exactly_n(root, n, acc);
        return acc;
    }
    bst() = default;
    bst(const bst& other)
            : root(copy(other.root)),
              m_size(other.m_size)
    {}
    // copy-and-swap
    bst& operator=(bst other) {
        std::swap(root, other.root);
        std::swap(m_size, other.m_size);
        return *this;
    }
    bst(bst&& other) noexcept {
        std::swap(root, other.root);
        std::swap(m_size, other.m_size);
    }
    bst(std::initializer_list<int> lst) {
        for(const int i : lst) {
            insert(i);
        }
    }
    void save(std::ostream& os) const {
        save(os, root);
    }
    int size() const {
        return m_size;
    }
    int height() const{
        return height(root);
    }
    void print_levels() const{
        for(int i = 1; i <= height(); ++i) {
            print_level(root, i);
            std::cout << std::endl;
        }
    }
    ~bst() {
        free(root);
    }
    void read(std::istream& is) {
        std::string number;
        while(is >> number) {
            int num;
            try{
                num = std::stoi(number);
            }
            catch(const std::exception& e) {
                std::cerr << e.what() << std::endl;
                return;
            }
            insert(num);
        }
    }
    bool erase_special(int k) {
        Node*& k_node = find(k);
        if(!k_node) {
            return false;
        }
        erase_even(k_node->left);
        erase_even(k_node->right);
        erase(k_node, k);
        m_size = weight(root);
        return true;
    }
private:
    // O(n)
    // O(n), we accumulate, so we don't even calculate weights twice
    // not that it matters to big O, constants don't matter
    int exactly_n(Node* root, int n, int& acc) {
        if(!root) {
            return 0;
        }
        int r = exactly_n(root->right, n, acc);
        int l = exactly_n(root->left, n, acc);
        if(r + l + 1 == n) {
            ++acc;
            return r + l + 1;
        }
        return r + l + 1;
    }
    static int height(const Node* root) {
        if(!root) {
            return 0;
        }
        return 1 + std::max(height(root->left), height(root->right));
    }
    static void print_level(const Node* root, int k) {
        if(k == 0 || !root) {
            return;
        }
        if(k == 1) {
            std::cout << root->value << " ";
        }
        else {
            print_level(root->left, k - 1);
            print_level(root->right, k - 1);
        }

    }
    Node*& find(int val) {
        return find(root, val);
    }
    bool insert(Node*& root, int val) {
        if(!root) {
            root = new Node{ val };
            return true;
        }
        if(val > root->value) {
            return insert(root->right, val);
        }
        else if (val < root->value)
            return insert(root->left, val);
        else {
            assert(false);
            return false;
        }
    }
    static Node*& find(Node*& root, int val) {
        if(!root || root->value == val) return root;
        if(val > root->value) {
            return find(root->right, val);
        }
        return find(root->left, val);
    }
    static Node* extract_min(Node*& root) {
        if(root->left) {
            return extract_min(root->left);
        }
        Node* n = root;
        root = root->left;
        return n;
    }
    static bool erase(Node*& root, int val) {
        if(!root) {
            return false;
        }
        if(root->value == val) {
            Node* to_delete = root;
            // if it has at most one child
            if(!root->left) {
                root = root->right;
            }
            else if(!root->right) {
                root = root->left;
            }
                // there is both a left child and right child
                // we can steal the min node on the right
            else {
                Node* min_right = extract_min(root->right);
                min_right->left =  root->left;
                min_right->right = root->right;
                root = min_right;
            }
            delete to_delete;
            return true;
        }

        if(root->value < val) {
            return erase(root->right, val);
        }
        return erase(root->left, val);
    }
    static int weight(const Node* root) {
        if(!root) return 0;
        return 1 + weight(root->left) + weight(root->right);
    }
    // O(n)
    static void erase_even(Node*& root) {
        if(!root) return;
        erase_even(root->left);
        erase_even(root->right);
        if(root->value % 2 == 0) {
            // delete the root
            erase(root, root->value);
        }
    }
    void free(Node*& root) {
        if(!root) return;
        free(root->left);
        delete root->left;
        root->left = nullptr;
        free(root->right);
        delete root->right;
        root->right = nullptr;
    }
    static void save(std::ostream& os, const Node* root) {
        if(!root) {
            return;
        }
        save(os, root->left);
        os << root->value << " ";
        save(os, root->right);
    }
    static Node* copy(const Node* other_root){
        if(!other_root) {
            return nullptr;
        }
        return new Node{ other_root->value,
                         copy(other_root->left),
                         copy(other_root->left) };

    }
};

#if 1
int main(int argc, char** argv) try {
    if(argc != 2) {
        std::cerr << "usage: <program> <filename>\n";
        return -1;
    }
    std::ifstream ifile(argv[1]);
    if(!ifile.is_open()) {
        std::cerr << "failed to open file\n";
        return -1;
    }
    bst t1;
    try{
        t1.read(ifile);
        std::cout << "The tree is: ";
        t1.save(std::cout);
        std::cout << std::endl;
        std::cout << "-------------------------------------------------------------\n";
        std::cout << "The number of subtrees with exactly 3 elements in them is: ";
        std::cout << t1.exactly_n(3) << std::endl;
        std::cout << "The number of subtrees with exactly 2 elements in them is: ";
        std::cout << t1.exactly_n(2) << std::endl;
        std::cout << "The number of subtrees with exactly 1 elements in them is: ";
        std::cout << t1.exactly_n(1) << std::endl;
        std::cout << "-------------------------------------------------------------\n";
        std::cout << "The tree after spacial erasure of node with value 5: ";
        t1.erase_special(5);
        t1.save(std::cout);
        std::cout << std::endl;
        std::cout << "The tree after spacial erasure of node with value 2: ";
        t1.erase_special(2);
        t1.save(std::cout);
        std::cout << std::endl;
        std::cout << "-------------------------------------------------------------\n";
        std::cout << "...Goodbye, press any key to exit.\n";
        char ch;
        std::cin.get(ch);
    }
    catch(const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
    }
    return 0;
}
catch(const std::exception& e) {
    std::cerr << "an unexpected error with message occurred: "
              << e.what();
}
#endif

#if 0
int main() {

    bst t{ 1, 5, 2, 7, 3, 4, 6, 0 };
    bst t1{ 1, 0, 2 };
    t.save(std::cout);
    std::cout << std::endl;
    t.print_levels();
    std::cout << std::endl;
    std::cout << "height " << t.height() << std::endl;
    std::cout << t.exactly_n(2) << std::endl;
    std::cout << "........................................\n";
    t.erase_special(5);
    std::cout << "weight: " << t.size() << std::endl;
    t.save(std::cout);
}
#endif

#endif //TASKS_TASK2_E1_H
