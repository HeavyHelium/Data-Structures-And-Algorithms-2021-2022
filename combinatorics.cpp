#include <iostream>

static void printArrHelper(int* arr, std::size_t size, std::size_t current)
{
    if(current == size) return;
    std::cout << arr[current] << " ";
    printArrHelper(arr, size, current + 1);
}

void printArr(int* arr, std::size_t size)
{
    printArrHelper(arr, size, 0);
    std::cout << std::endl;
}


void printPermutations(int* arr, std::size_t size, std::size_t cid = 0)
{
    if(cid == size - 1)
        printArr(arr, size);
    for(std::size_t i = cid; i < size; ++i)
    {
        std::swap(arr[i], arr[cid]);
        printPermutations(arr, size, cid  + 1);
        std::swap(arr[i], arr[cid]);
    }
}

/// Exponential
void printCombinations(int* arr, int* current, std::size_t size, std::size_t cid = 0, std::size_t arrInd = 0)
{
    if(arrInd == size)
    { if(cid == 3)printArr(current, cid); return; }/// k == 3, done stupidly just cuz
    printCombinations(arr, current, size, cid, arrInd + 1);
    current[cid] = arr[arrInd];
    printCombinations(arr, current, size, cid + 1, arrInd + 1);

}

void printCombinationsSmart(int* arr, int* current, std::size_t size, std::size_t k, std::size_t cid = 0, std::size_t start = 0)
{
    if(cid == k)
    {
        printArr(current, cid);
        return;
    }

    for(std::size_t i = start; i < size; ++i)
    {
        current[cid] = arr[i];
        printCombinationsSmart(arr, current, size, k, cid + 1, i + 1);
    }
}


int main()
{
    int arr[] = { 1, 2, 3, 4 };
    std::size_t size = sizeof(arr)/sizeof(*arr);
    int* comb = new int[size];
    printArr(arr, size);
    //printCombinations(arr, comb, size);
    std::cout << "..................." << std::endl;
    printCombinationsSmart(arr, comb, size, 3);
    delete comb;
    return 0;
}
