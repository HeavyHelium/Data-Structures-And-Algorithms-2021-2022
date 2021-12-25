#include <iostream>
#include "input_handler.h"
int main()try
{
    input_handler ih;
    ih.getInput();
    return 0;
}
catch(const std::exception& e)
{ std::cerr << "error: " << e.what(); }