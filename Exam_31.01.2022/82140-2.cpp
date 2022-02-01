//
// СУ "Св. Климент Охридски"
// Факултет по математика и информатика
//
// Курсове Структури от данни и програмиране
//         Структури от данни
// Зимен семестър 2021/22 г.
//
// Практически изпит
//
// Име: Диана Венциславова Маркова
// ФН: 82140
// Специалност: Компютърни науки
// Курс: 2(втори)
// Административна група: 1(първа)
// Ден, в който се явявате на изпита: 31.01.2022(понеделник)
// Начален час на изпита: 09:00
// Кой компилатор използвате: g++
//
//#define _CRT_SECURE_NO_WARNINGS

#include <cassert>
#include <iostream>
#include <stdexcept>
//#include <fstream>
//#include <cstddef>
//#include <algorithm>
//#include <utility>
//#include <vector>
//#include <string>
#include <queue>
#include <list>

class tree {
    struct node {
        int val = 0;
        std::list<node*> children;
    };
    node* root;
    int m_size = 0;
public:
    tree() = default;
    tree(const tree& other) = delete;
    tree& operator=(const tree& other) = delete;
    tree(std::istream& is) {
        std::string line;
        std::getline(is, line);
        const char* l = line.c_str();
        assert_p(*l);
        ++l;
        skip_white_space(l);
        int val = extract_int(l);
        skip_white_space(l);
        assert_p(*l);
        ++l;
        if(*l) {
            throw std::logic_error("bad input");
        }
        root = new node{ val };
        ++m_size;
        std::queue<node*> children_level;
        children_level.push(root);
        //children_level.push(nullptr);
        while(!std::getline(is, line)) {
            const char* l = line.c_str();
            assert_p(*l);
            ++l;
            if(!l) break;
            while(*l && !children_level.empty()) {
                node* top = children_level.front();
                children_level.pop();

                if(!top) {
                    read_children(top, l, children_level);
                     assert_p(*l);
                    ++l;
                }
            }
            if(*l || !children_level.empty()) {
                free(root);
                throw std::invalid_argument("bad input");
            }
        }
    }
    bool contains(tree& other) {
        contains_helper(root, other.root);
    }
    // size needs to be fixed later
    void remove_occurrences(tree& other) {
        remove_occurrences_helper(root, other.root);
    }
    void save_to_file(std::ostream& file) {
        std::queue<node*> q;
        q.push(root);
        q.push(nullptr);
        file << '|';
        while(!q.empty()) {
            node* top = q.front();
            q.pop();
            if(!top) {
                if(q.empty()) {
                   return;
                }
                file << '\n';
                q.push(nullptr);
            } else {
                for(node* child : top->children)  {
                    file << " " << child->val << " ";
                    q.push(child);
                }
                file << '|';
            }
        }
    }
    ~tree() {
        free(root);
    }
private:
    bool contains_helper(node* root, node* other_root) {
        if(!root) {
            return false;
        }
        for(node* child : root->children) {
            if(contains_helper(child, other_root)) {
                return true;
            }
        }
        return are_equal(root, other_root);
    }
    void remove_occurrences_helper(node* root, node* other_root) {
        if(!root) {
            return;
        }
        std::list<node*>::iterator iter = root->children.begin();
        while(iter != root->children.end()) {
            if(contains_helper(*iter, other_root)) {
                free(*iter);
                std::list<node*>::iterator temp = iter;
                ++iter;
                root->children.erase(temp);
                return;
            }
        }
        return;
    }
    bool are_equal(node* root, node* other_root) {
        if(!root && root == other_root) {
            return true;
        }
        else if(!other_root && root == other_root) {
            return true;
        } else if(!root || !other_root) {
            return false;
        } else {
            if(root->val != other_root->val) return false;
            if(root->children.size() != other_root->children.size())
                return false;
            root->children.sort([](node* a, node* b) { return a->val < b->val; });
            other_root->children.sort([](node* a, node* b) { return a->val < b->val; });
            std::list<node*>::iterator iter1 = root->children.begin();
            std::list<node*>::iterator iter2 = other_root->children.begin();
            std::list<node*>::iterator end1 = other_root->children.end();
            while(iter1 != end1) {
                if(!are_equal(*iter1, *iter2)) {
                    return false;
                }
            }
        }
        return true;
    }
    static void free(node* tree) {
        if(!tree) return;
        for(node* child : tree->children) {
            free(child);
        }
        delete tree;
    }
    static void skip_white_space(const char*& str) {
        while(*str && *str == ' ' ) {
            ++str;
        }
    }
    bool is_digit(char ch) {
        return ch <= 'z' && ch >= 'a';
    }
    int extract_int(const char*& str) {
        if(!str) {
            throw std::logic_error("no int");
        }
        int res = 0;
        while(is_digit(*str++)) {
            res *= 10;
            res += *str - '0';
        }
        return res;
    }
    void assert_p(char ch) {
        if(ch != '|') {
            free(root);
            throw std::invalid_argument("bad input");
        }
    }
    void read_children(node* r, const char*& l, std::queue<node*>& q) {
        skip_white_space(l);
        if(!*l) {
            return;
        }
        while(*l != '|') {
            skip_white_space(l);
            int ch = extract_int(l);
            node* new_n = new node { ch } ;
            r->children.push_back(new_n);
            q.push(new_n);
            ++m_size;
        }
    }
};


int main(int argc, char** argv) try {
    if(argc != 3) {
        std::cerr << "usage: <file1> <file2> <file3>\n";
        return -1;
    }

    return 0;
} catch(const std::exception& e) {
    std::cerr << "the following error occurred: ";
    std::cerr << e.what() << '\n';
    return -1;
}
