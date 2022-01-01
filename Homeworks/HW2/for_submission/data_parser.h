#ifndef HW2_1_DATA_PARSER
#define HW2_1_DATA_PARSER
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstddef>
#include <cassert>
#include "string_operations.h"

struct manager_subordinate
{
    std::string manager = "";
    std::string subordinate = "";
    friend std::ostream& operator<<(std::ostream& os,
                                    const manager_subordinate& p);
};

class data_parser
{
    std::vector<manager_subordinate> ms_pairs;
    std::size_t line = 1;
public:
    data_parser() = default;
    static std::string console_data();
    static std::string file_data(const std::string& path);
    void parse_text(const std::string& text);
    const std::vector<manager_subordinate>& pairs();
    void clear_states();
private:
    static std::vector<std::string> parse_line(const char*& text);
    static std::string file_to_string(const std::string& path);
};

#endif //HW2_1_DATA_PARSER

