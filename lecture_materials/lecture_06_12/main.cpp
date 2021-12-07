#include <iostream>

/// Binary trees

template<typename T>
class binary_tree
{
    struct Node
    {
        T data;
        Node* left;//smaller usualy
        Node* right;//larger usually
        // Node* parent; -- allows us to go backwards(+)
        // makes insertion and deletion O(1)(+)
        // takes more memory and makes implementation more complex(-)
    };

};

template<typename T>
class binary_search_tree
{
    struct Node
    {
        T data;
        Node* left;
        Node* right;
    };
};

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
