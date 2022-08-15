#include <iostream>
#include <fstream>
//#define GENERATE_FILE
//#define SAVE_RESULTS
#include "input_output_handler.h"

void generate_words(std::ostream& os, std::size_t cnt = 18'000'000, bool modify = false)
{
    bool even = true;
    while(cnt--)
    {
        os << "supercalifragilisticexpialidocious even though "
                       "the sound of it is something quite atrocious\n";
        os << "Xenotransplantation, Pulchritudinous, Trichotillomania";
        if(modify) os << " a distinct touch";
        os << "\n";
        even = !even;
    }

}

int main(int argc, char** argv)try {
#ifdef GENERATE_FILE
    const int word_cnt = 3'000'000;
    std::ofstream test_file("f1_2.txt");
    generate_words(test_file, word_cnt, false);
    std::ofstream test_file1("f2_2.txt");
    generate_words(test_file1, word_cnt / 2, false);
#else
    if(argc != 3) {
        std::cerr << "usage: <program> <filename1> <filename2>\n";
        return -1;
    }
    std::ifstream file1(argv[1]);
    std::ifstream file2(argv[2]);

    if(!file1.is_open()) {
        std::cerr << "error: failed to open file with path: \""
                  << argv[1] << "\"\n";
        return -1;
    }
    if(!file2.is_open()) {
        std::cerr << "error: failed to open file with path: \""
                  << argv[2] << "\"\n";
        return -1;
    }

    input_output_handler h;
    h.handle_input(file1, file2);
    h.handle_output(argv[1], argv[2]);
    input_output_handler::say_goodbye();
#endif
    return 0;
}
catch(const std::exception& e) {
    std::cerr << "an unexpected error occurred with message: "
              << e.what() << "\n";
}

