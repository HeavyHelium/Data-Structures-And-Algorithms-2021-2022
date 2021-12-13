#include "binary_tree.h"
int main()
{
    tree<int> t;
    int path[] = { -1 };
    t.insert(1, path);
    int path1[] = { 0, 0, -1 };
    t.insert(2, path);
    t.print();
    return 0;
}

