#include <iostream>
#include <cassert>
#include <queue>
#include <string_view>
#include <typeinfo>

class binary_tree {
    struct node {
        int value = 0;
        node* l{ nullptr };
        node* r{ nullptr };
    };

    node* root = nullptr;
    int m_size = 0;
public:
    binary_tree() = default;
    binary_tree(const binary_tree& other) = delete;
    void operator=(const binary_tree& other) = delete;
    ~binary_tree() {
        free_that_tree(root);
    }
    void add_node(int value) {
        add_node(root, value);
        ++m_size;
    }
    std::string serialize() const {
        if(!root) return {};
        return serialize_helper(root);
    }
    std::string inorder() const {
        return inorder(root);
    }
    void deserialize(const std::string& content) {
        assert(!root);
        const char* text = content.c_str();
        skip_whitespace(text);
        if(!*text) {
            return;
        }
        deserialize_helper(root, text);
    }
private:
    void free_that_tree(node* root) {
        if(!root) {
            return;
        } else {
            free(root->l);
            free(root->r);
            delete root;
        }
    }
    static void deserialize_helper(node*& root, const char*& content) {
         skip_whitespace(content);
         if(!*content) {
            return;
         }
         int val = extract_int(content);
         if(val == -1) {
            return;
         }
         root = new node{ val };
         deserialize_helper(root->l, content);
         deserialize_helper(root->r, content);
    }
    static std::string serialize_helper(const node* root) {
        if(!root) {
            return std::to_string(-1);
        }
        return std::to_string(root->value) +
               " " +
               serialize_helper(root->l) +
               " " + serialize_helper(root->r);
    }
    static std::string inorder(const node* root) {
        if(!root) return {};
        return inorder(root->l) + " " +
               std::to_string(root->value) + " " +
               inorder(root->r);
    }
    // adds as if it were a binary search tree
    void add_node(node*& root, int value) {
        if(!root) {
            root = new node { value };
        } else if(value < root->value) {
                add_node(root->l, value);
        } else add_node(root->r, value);
    }
public:
    /// Helper functions for parsing
    static void skip_whitespace(const char*& line) {
        while(is_whitespace(*line)) {
            ++line;
        }
    }
    static bool is_whitespace(const char ch) {
        return ch == ' '  ||
               ch == '\n' ||
               ch == '\t';
    }
    static bool is_digit(const char ch) {
        return ch <= '9' &&
               ch >= '0';
    }
    static int extract_int(const char*& line) {
       skip_whitespace(line);

       int sign = 1;
       int number = 0;
       const char* before = line;

       if(*line == '-') {
            sign = -1;
            ++line;
       }
       if(!is_digit(*line)) {
            line = before; // might have skipped that, but just in case
            throw std::runtime_error("wrong input");
       } else {
           const char* begin = line;
           while(is_digit(*line)) {
              number *= 10;
              number += *line - '0';
              ++line;
           }
           number *= sign;
           return number;
       }
    }
};

std::ostream& operator<<(std::ostream& os, const binary_tree& t) {
    return os << t.serialize();
}

std::istream& operator>>(std::istream& is, binary_tree& t) {
    std::string line;
    std::getline(is, line);
    t.deserialize(line);
    return is;
}

int main() try {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    binary_tree t;
    /*
    t.add_node(5);
    t.add_node(3);
    t.add_node(7);
    t.add_node(1);
    t.add_node(4);
    t.add_node(6);*/
    /*
    const char* str = "5 3 1 -1 -1 4 -1 -1 7 6 -1 -1 -1";
    std::cout << binary_tree::extract_int(str) << "\n";
    std::cout << str << "\n";
    std::cout << binary_tree::extract_int(str) << "\n";
    std::cout << binary_tree::extract_int(str) << "\n";*/

    std::cin >> t;
    std::cout << t.inorder() << '\n';
    std::cout << t << std::endl;

    return 0;
} catch (const std::exception& e) {
    std::cerr << "something went terribly wrong: \n" <<
                  typeid(e).name() << " with message: " <<
                 e.what() << '\n';
}
