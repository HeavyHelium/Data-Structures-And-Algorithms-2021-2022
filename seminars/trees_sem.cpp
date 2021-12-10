#include <cassert>
#include <iostream>
#include <algorithm>

struct node
{
    node* left = nullptr;
    node* right = nullptr;
    int value;
};


int size(node* n)
{
    if(n)
        return 1 + size(n->left) + size(n->right);
    return 0;
}

int height(node* n)
{
    if(n)
        return 1 + std::max(height(n->left), height(n->right));
    return 0;
}

void insert(node*& n, int newValue)
{
    if(!n)
        n = new node{ nullptr, nullptr, newValue };
    const int r = rand();
    if(r % 2 == 0)
        insert(n->left, newValue);
    else
        insert(n->right, newValue);
}

node* find(node* n, int value)
{
    if(n)
    {
        if (n->value == value) return n;
        node* res = find(n->left, value);
        if(!res)
            res = find(n->right, value);
        return res;
    }
    return nullptr;
}

void erase(node* n, int value)
{
    node* toErase = find(n, value);
    if(toErase)
}

void free(node* n)
{
    if(n)
    {
        free(n->left);
        free(n->right);
    }
    delete n;
}

int main()
{
    std::cout << std::endl;
    return 0;
}

/*
/// does not cover nullptr
int size(node& n)
{
    int sum = 0;
    if(n.left)
        sum += size(*n.left);
    if(n.right)
        sum += size(*n.right);
    return sum;
}
*/
