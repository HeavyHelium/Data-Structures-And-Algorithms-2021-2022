#include <iostream>
#include <queue>
#include <stack>


bool isOperator(char ch)
{ return ch == '+' || ch == '-' || ch == '*' || ch == '/'; }

///@brief to postfix style
void to_reverse_polish(std::vector<char> expr)
{
    // 6 + 4 / 2 + 3
    std::stack<char> operator_stack;
    std::queue<char> output_queue;
    for(std::size_t i = 0; i < expr.size(); ++i)
    {
        if(isOperator(ch) && !operator_stack.empty() && operator_stack.top())
        {

        }
    }
}



int main()
{
    return 0;
}
