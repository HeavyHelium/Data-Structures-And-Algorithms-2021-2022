#pragma once
#ifndef HW3_1_INPUT_OUTPUT_HANDLER_H
#define HW3_1_INPUT_OUTPUT_HANDLER_H
#include <cstddef>
#include <fstream>
#include "interface.h"

class input_output_handler {
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
    static void say_goodbye();
};

#endif
