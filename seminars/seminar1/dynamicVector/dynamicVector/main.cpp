#include <iostream>
#include "dynamicVector.h"

int main()
{
    dynamicArray<int> arr1;
    arr1.push_back(10);
    arr1.push_back(-2);

    std::cout << arr1.size()<< std::endl;

    for(auto iter = arr1.begin(); iter != arr1.end(); ++iter)
        std::cout << *iter << " ";
    std::cout << std::endl;

    for(int i: arr1)
        std::cout << i << " ";
    std::cout << std::endl;

    return 0;
}
