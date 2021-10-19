#include <iostream>


enum type
{
    unknown,
    yellow,
    green,
    red
};

template<class T>
class apple
{
    T index = 16;
    type color;
    std::size_t weight;
public:
    apple(type color, std::size_t weight)
        : color(color), weight(weight)
    {}
    template<class T1>
    apple(const T1& other)
        : color(other.color), weight(other.weight), index(other.index)
    { std::cout << "my function!" << std::endl; }
};


int main()
{
    apple<int> a1(yellow, 250);
    apple a2(a1);
    return 0;
}
