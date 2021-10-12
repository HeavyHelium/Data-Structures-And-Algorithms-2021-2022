#include <iostream>
#include <ctime>


int main()
{
    const std::size_t rowsCnt = 6000;
    const std::size_t colsCnt = 400000;

    int* arr = new int[rowsCnt * colsCnt];


    long long sum = 0;
    std::time_t start, end;

    std::cout << "Initializing members of the array...";

    start = std::time(NULL);

    /// initialize array elements
    for(std::size_t row = 0; row < rowsCnt; ++row)
        for(std::size_t col = 0; col < colsCnt; ++col)
            arr[row * colsCnt + col] = static_cast<int>(row);

    end = std::time(NULL);

    std::cout << "\n execution took " << end - start << " seconds" << std::endl;


    /// Iterate over the rows and columns

    start = std::time(NULL);

    std::cout << "iterating over rows and then columns...";

    for(std::size_t i = 0; i < rowsCnt; ++i)
    {
        for(std::size_t j = 0; j < colsCnt; ++j)
            sum += arr[i * colsCnt + j];
    }

    end = std::time(NULL);

    std::cout << "\n execution took " << end - start << " seconds" << std::endl;
    std::cout << "(sum here is  " << sum << ')' <<std::endl;

    sum = 0;
    start = std::time(NULL);

    std::cout << "Iterating over columns and then rows...";

    for(std::size_t j = 0; j < colsCnt; ++j)
    {
        for(std::size_t i = 0; i < rowsCnt; ++i)
        {
            sum += arr[i*colsCnt + j];

        }
    }

    end = std::time(NULL);

    std::cout << "\n execution took " << end - start << std::endl;
    std::cout << "(sum here is  " << sum << ')' <<std::endl;

}
