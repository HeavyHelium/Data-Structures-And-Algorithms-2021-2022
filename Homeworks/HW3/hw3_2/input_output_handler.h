#pragma once
#ifndef HW3_1_INPUT_OUTPUT_HANDLER_H
#define HW3_1_INPUT_OUTPUT_HANDLER_H
#include <cstddef>
#include <fstream>
#include "interface.h"

class input_output_handler {
    ///TODO: Refactor, make them pairs
    std::size_t total_common = 0;

    std::size_t fst_unique = 0;
    std::size_t snd_unique = 0;

    std::size_t fst_total = 0;
    std::size_t snd_total = 0;

    double similarity_rate1 = 0;
    double similarity_rate2 = 0;
    ComparisonReport r;
public:
    input_output_handler() = default;
    void handle_input(std::ifstream& ifile1, std::ifstream& ifile2);
    void handle_output(const char* filename1,
                       const char* filename2) const;
    void save_as_hashed_and_sorted(const ComparisonReport& rep,
                                          const char* filepath1 = "u1.txt",
                                          const char* filepath2 = "u2.txt",
                                          const char* filepath3 = "c.txt");
    static void say_goodbye();
private:
    void handle_output_helper(const char* filename1,
                       const char* filename2, const ComparisonReport& r) const;
};


#endif //HW3_1_INPUT_OUTPUT_HANDLER_H
