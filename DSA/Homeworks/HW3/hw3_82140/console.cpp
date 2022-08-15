#include <iostream>
#include "input_output_handler.h"

int main(int argc, char** argv)try {
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
    return 0;
}
catch(const std::exception& e) {
    std::cerr << "an unexpected error occurred with message: "
              << e.what() << "\n";
}
