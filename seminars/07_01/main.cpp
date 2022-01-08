#include <iostream>
#include <vector>

struct Student{
    int fn;
    std::string name;
};

std::vector<std::pair<int, std::string>> students;

bool findStudent(const std::pair<int, std::string>& check)
{
    for(std::size_t c = 0; c < students.size(); ++c)
        if(check.first == students[c].first)
            return true;
    return false;
}

int main() {

    return 0;
}
