#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>

std::size_t numDigitsAfterMult(std::size_t number1, std::size_t number2)
{
    return (std::floor(std::log10(number1) + std::log10(number2)) + 1);
}

void printPrimeFactors(std::size_t n)
{
    while(! (n % 2))
    {
        std::cout << 2 << " ";
        n /= 2;
    }
    std::size_t limit = std::sqrt(n);
    for(std::size_t i = 3; i < limit; ++i)
    {
        while(!(n % i))
        {
            std::cout << i << " ";
            n /= i;
        }
    }
}

bool isPrime(std::size_t n)
{
    std::size_t limit = std::sqrt(n);
    if(n == 1 || n == 0) return false;
    for(std::size_t i = 2; i <= limit; ++i)
        if(!(n % i)) return false;
    return true;
}

void printFirstNPrime(std::size_t n)
{
    std::size_t cnt = 0;
    std::size_t i = 2;
    while(cnt < n)
    {
        if(isPrime(i))
        {
            std::cout << i << " ";
            ++cnt;
        }
        ++i;
    }
}

std::size_t fibHelper(std::size_t current, std::size_t n, std::size_t f1, std::size_t f2)
{
    if(current == n) return f2;
    return fibHelper(current + 1, n, (f1 + f2), f1);

}
std::size_t nthFib(std::size_t n)
{
    return fibHelper(0, n, 1, 0);
}

struct matSize
{
    std::size_t x, y;
    bool operator==(const matSize& other) const
    {
        return x == other.x && y == other.y;
    }
};

std::vector<std::vector<int>> matrixSum(int arr1[][3], int arr2[][3], const matSize& size)
{
    std::vector<std::vector<int>> sum(size.x, std::vector<int>(size.y));
    for(std::size_t i = 0; i < size.x; ++i)
    {
        for(std::size_t j = 0; j < size.y; ++j)
            sum[i][j] = arr1[i][j] + arr2[i][j];
    }
    return sum;
}

//(m*n)(n*k)

///
std::vector<std::vector<int>> matrixProduct(int arr1[][3], int arr2[][2], const matSize& size1, const matSize& size2)
{
    assert (size1.y == size2.x);
    std::vector<std::vector<int>> product(size1.x, std::vector<int>(size2.y));
    for(std::size_t i = 0; i < size1.x; ++i)
    {
        for(std::size_t j = 0; j < size2.y; ++j)
        {
            for(std::size_t k = 0; k < size1.y; ++k)
                    product[i][j] += arr1[i][k] * arr2[k][j];
        }
    }
    return product;
}


int main()
{
    std::cout << numDigitsAfterMult(123456, 25) << std::endl;
    std::cout << std::log10(9999) << std::endl;
    printPrimeFactors(315 * 2);
    std::cout << std::endl;
    printFirstNPrime(10);
    std::cout << std::endl;

    for(std::size_t i = 0; i < 10; ++i)
        std::cout << nthFib(i) << " ";
    std::cout << std::endl << std::endl;

    int arr1[4][3] = {
        {5, -5, 2},
        {1, 4, 7},
        {2, 1, 3},
        {-3, -6, 11}
    };

    int arr2[4][3] = {
        {15, -8, 2},
        {1, 3, 2},
        {-1, -1, -3},
        {44, 6, 4}
    };

    auto sum = matrixSum(arr1, arr2, {4, 3});

    for(auto i = sum.begin(); i < sum.end(); ++i)
    {
        for(auto j = i->begin(); j < i->end(); ++j)
            std::cout << *j << " ";
        std::cout << std::endl;
    }

    std::cout << std::endl;

    int mat1[2][3] = { 1, 2, 3, 4, 5, 6 };
    int mat2[3][2] = { 10, 11, 20, 21, 30, 31 };
    auto product = matrixProduct(mat1, mat2, { 2, 3 }, { 3, 2 });

    for(auto i = product.begin(); i < product.end(); ++i)
    {
        for(auto j = i->begin(); j < i->end(); ++j)
            std::cout << *j << " ";
        std::cout << std::endl;
    }

    return 0;
}
