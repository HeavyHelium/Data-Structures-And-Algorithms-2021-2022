#include <iostream>
#include <algorithm>

void quickSort(int* arr, int beg, int end)
{
    if(beg >= end) return;

    int read = beg;
    int write = beg - 1;
    //end is pivot
    while(read < end)
    {
        if(arr[read] < arr[end])
        {
            ++write;
            std::swap(arr[write], arr[read]);
        }
        ++read;
    }
    std::swap(arr[end], arr[write + 1]);
    quickSort(arr, beg, write);
    quickSort(arr, write + 2, end);
}


void copy(int* beg, int* end, int* copied)
{
    while(beg < end)
        *(copied++) = *(beg++);
}

/*
int* merge(int* beg1, int* end1, int* beg2, int* end2)
{
    int* result = new int[end1 - beg1 + end2 - beg2];
    int* temp = result;
    int* temp = beg1;

    while(beg1 <= end1 && beg2 < end2)
        *result++ = (*beg1 < *beg2) ? *(beg2++) : *(beg1++);
    copy(beg1, end1, result);
    copy(beg2, end2, result);
    return result;
}*/

/*
int* mergeSort(int* beg, int* end, int* res)
{
    if(beg >= end) return res;

    int* middle = beg + (end - beg) / 2;
    int* size1 = middle - beg;
    int* size2 = end - middle;

    int* half1 = mergeSort(beg, middle);
    int* half2 = mergeSort(middle, end);

    int* result = mergeSort(half1, half1 + size1, half2, half2 + size2);
    delete[] half1;
    delete[] half2;

    return result;
}*/

int main()
{
    int arr1[] = { 1, 3, 5, 7 };
    std::size_t size1 = sizeof(arr1)/sizeof(*arr1);

    int arr2[] = { 2, 4, 6, 8 };
    std::size_t size2 = sizeof(arr2)/sizeof(*arr2);

//    int* merged = merge(arr1, arr1 + 3, arr2, arr2 + 3);

//    for(std::size_t i = 0; i < 8; ++i)
//        std::cout << merged[i] << " ";
//    std::cout << std::endl;

    int arr[101];
    std::size_t groupsNumber;
    std::cin >> groupsNumber;

    std::size_t temp = 0;
    while(temp < groupsNumber && std::cin >> arr[temp++])
    ;

    quickSort(arr, 0, groupsNumber - 1);

    for(std::size_t i = 0; i < groupsNumber; ++i)
        std::cout << arr[i] << " ";
    std::cout << std::endl;

    std::size_t res = 0;

    for(std::size_t i = 0; i < groupsNumber / 2 + 1; ++i)
        res += arr[i] / 2 + 1;

    std::cout << res << std::endl;

}

/*
5
3 5 9 11 7

*/
