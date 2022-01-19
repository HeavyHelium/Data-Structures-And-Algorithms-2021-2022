#ifndef TASKS_TASK2_E2_H
#define TASKS_TASK2_E2_H

#include <iostream>
#include <cstddef>
#include <list>
#include <queue>
#include <algorithm>
#include <cassert>
#include <string>

class tree{
    struct Node{
        int value;
        std::list<Node*> children;
    };
    Node* root = nullptr;
    std::size_t m_size = 0;
public:
    tree() = default;
    tree(const tree& other)
            : root{copy(root)}, m_size(other.m_size)
    {}
    ~tree() {
        free(root);
    }
    tree(tree&& other) {
        std::swap(other.root, root);
        std::swap(m_size, other.m_size);
    }
    void operator=(const tree& other) = delete;
    void insert(const int parent_key, const int child) {
        Node* found_parent = find_node(root, parent_key);
        if(!found_parent) {
            throw std::invalid_argument("No such node in tree");
        }
        assert(found_parent->value == parent_key);
        found_parent->children.push_back(new Node{ child });
        ++m_size;
    }
    void insert_root(const int value) {
        assert(!root);
        root = new Node{ value };
        ++m_size;
    }
    std::vector<std::vector<int>> tree_by_levels() const {
        std::vector<std::vector<int>> v{};
        if(!root) return v;
        v.push_back(std::vector<int>{});
        std::queue<const Node*> q;
        std::size_t level = 0;
        q.push(root);
        q.push(nullptr);
        while(!q.empty()) {
            const Node* current = q.front();
            q.pop();
            if(!current) {
                if(q.empty()) {
                    return v;
                }
                v.push_back(std::vector<int>{});
                ++level;
                q.push(nullptr);
            }
            else {
                v[level].push_back(current->value);
                for(const Node* child : current->children) {
                    q.push(child);
                }
            }
        }
    }
    void print() const {
        print_bfs(root);
    }
    Node* get_parent(const int value) {
        return get_parent(root, value);
    }
    // O(nlogn + mlogm)
    bool can_be_transformed(const tree& other) const {
        std::vector<std::vector<int>> v1 = tree_by_levels();
        std::vector<std::vector<int>> v2 = other.tree_by_levels();
        if(v1.size() != v2.size()){
            return false;
        }
        for(std::size_t i = 0; i != v1.size(); ++i) {
            if(v1[i].size() != v2[i].size()){
                return false;
            }
            std::sort(v1[i].begin(), v1[i].end());
            std::sort(v2[i].begin(), v2[i].end());
            for(std::size_t j = 0; j < v1[i].size(); ++j) {
                if(v1[i][j] != v2[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
private:
    Node* find_node(Node* root, const int key) {
        if(!root) return root;
        if(root->value == key) {
            return root;
        }
        //std::cout << "here\n";
        for(Node* child : root->children) {
            Node* depth = find_node(child, key);
            if(depth) {
                return depth;
            }
        }
        return nullptr;
    }
    void free(Node* root) {
        if(!root) return;
        for(Node* child : root->children) {
            free(child);
        }
        delete root;
    }
    Node* get_parent(Node* root, const int key) {
        if(!root || root->value == key)
            return nullptr;
        for(auto child : root->children) {
            if(child->value == key) {
                return root;
            }
            Node* depth = get_parent(child, key);
            if(depth)
                return depth;
        }
        return nullptr;
    }
    Node* copy(const Node* other_root) {
        if(!other_root) {
            return nullptr;
        }
        Node* result = new Node{ other_root->value };
        for(Node* other_child : other_root->children) {
            result->children.push_back(copy(other_child));
        }
    }
    static void print_bfs(const Node* root, std::ostream& os = std::cout) {
        if(!root) {
            return;
        }
        std::queue<const Node*> q;
        q.push(root);
        q.push(nullptr);
        while(!q.empty()) {
            const Node* current = q.front();
            q.pop();
            if(!current) {
                if(q.empty()) {
                    return;
                }
                std::cout << std::endl;
                q.push(nullptr);
            }
            else {
                std::cout << current->value << " ";
                for(auto child : current->children) {
                    q.push(child);
                }
            }
        }

    }
};

void skip_whitespace(const char*& line) {
    while(*line == ' ' || *line == '\t' || *line == '\r') {
        ++line;
    }
}

struct string_slice {
    const char* beg = nullptr;
    std::size_t len = 0;
};

string_slice extract_word(const char*& text, char delimiter = '-') {
    skip_whitespace(text);
    const char* begin = text;
    while(*text && *text != delimiter) {
        ++text;
    }
    return { begin, (std::size_t)(text - begin) };
}

tree read_tree() {
    int root;
    std::cin >> root;
    std::cin.ignore();
    tree res;
    res.insert_root(root);
    std::string line;
    while(std::getline(std::cin, line)) {
        const char* l = line.c_str();
        string_slice word1 = extract_word(l);
        if(word1.len == 0) {
            throw std::invalid_argument("wrong input format");
        }
        else if(!*l) {
            throw std::invalid_argument("wrong input format");
        }
        else {
            int number1 = std::stoi(std::string(word1.beg, word1.beg + word1.len));
            assert(*l == '-');
            ++l;
            string_slice word2 = extract_word(l, '\0');
            int number2 = std::stoi(std::string(word2.beg, word2.beg + word2.len));
            res.insert(number1, number2);
        }
    }
    return res;
}

int main() try {
    tree t;
    t.insert_root(5);
    t.insert(5, 2);
    t.insert(5, 3);
    t.insert(5, 10);
    t.insert(10, 7);
    t.insert(10, 8);
    t.insert(7, 9);
    t.insert(9, 15);
    t.insert(2, 11);
    //---------------
    tree t2;
    t2.insert_root(5);
    t2.insert(5, 3);
    t2.insert(5, 2);
    t2.insert(5, 10);
    t2.insert(3, 7);
    t2.insert(3, 8);
    t2.insert(7, 9);
    t2.insert(9, 15);
    t2.insert(10, 11);
    /*
     5
     5-3
     5-2
     5-10
     3-7
     3-8
     7-9
     9-15
     10-11
     * */
    //---------------
    tree t3;
    t3.insert_root(7);
    t3.insert(7, 3);
    t3.insert(7, 2);
    t3.insert(7, 10);
    t3.insert(3, 14);
    t3.insert(3, 8);
    t3.insert(7, 9);
    t3.insert(9, 15);
    t3.insert(10, 11);
    //---------------
    // input format: root
    // input every child as parent-child, where parent must be present in the hierarchy beforehand
    // e.g
    // 7
    // 7 - 3
    // 7 - 2
    // 7 - 10
    // 3 - 14
    // 3 - 8
    // 7 - 9
    // 9 - 15
    // 10 - 11

    std::cout << "input for tree1:\n";
    tree tree1(read_tree());
    std::cout << "successfully loaded tree1!\n";
    std::cin.clear();
    std::cout << "\n";
    std::cout << "input for tree2:\n";
    tree tree2(read_tree());
    std::cout << "successfully loaded tree2!\n\n";
    std::cin.clear();

    std::cout << "tree1 can be transformed to tree2: " << std::boolalpha << tree1.can_be_transformed(tree2) << std::endl;

    std::cout << "\n...Goodbye! Press any key to exit the program. \n";
    char ch;
    std::cin.get(ch);
    /*
    std::cout << "parent of 9: " << t.get_parent(9)->value << std::endl;

    auto b_l = t.tree_by_levels();
    for(auto vec : b_l) {
        for(auto elem : vec) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }*/
    //std::cout << std::boolalpha << t.can_be_transformed(t2) << std::endl;
    //std::cout << std::boolalpha << t.can_be_transformed(t3) << std::endl;
}
catch(const std::exception& e) {
    std::cout << e.what() << std::endl;
    return -1;
}

#endif //TASKS_TASK2_E2_H
