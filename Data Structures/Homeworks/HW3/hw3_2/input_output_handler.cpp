#include <iostream>
#include <cmath>
#include "input_output_handler.h"
#include "interface.h"

void input_output_handler::handle_input(std::ifstream& ifile1, std::ifstream& ifile2) {
    Comparator c;
    r = c.compare(ifile1, ifile2);

    ifile1.close();
    ifile2.close();

    total_common = r.commonWords.total_word_cnt();
    fst_unique = r.uniqueWords[0].total_word_cnt();
    fst_total  = total_common + fst_unique;
    snd_unique = r.uniqueWords[1].total_word_cnt();
    snd_total  = total_common + snd_unique;

    similarity_rate1 = total_common * 100.0 / fst_total;
    similarity_rate2 = total_common * 100.0 / snd_total;
}
void input_output_handler::handle_output(const char* filename1,
                                         const char* filename2) const {
    std::cout << "Successfully analyzed files!\n\n";

    std::cout << "\t" << filename1 << " contains a total of " << fst_total
              << " words and " << total_common << " of them are also contained "
              << "in " << filename2 << ". \n"
              << "\tSimilarity rate: " << std::floor(similarity_rate1)
              << " %.\n";

    std::cout << std::endl;

    std::cout << "\t" << filename2 << " contains a total of " << snd_total
              << " words and " << total_common << " of them are also contained "
              << "in " << filename1 << ".\n"
              << "\tSimilarity rate: " << std::floor(similarity_rate2)
              << " %.\n\n";

    std::cout << "\t---------------------------------------------------\n";
    std::cout << "\tCommon words: \n";
    std::cout << "\t---------------------------------------------------\n";
    for(const std::string& word : r.commonWords.words()) {
        std::cout << "\t" << word << "\n";
    }

    std::cout << "\t---------------------------------------------------\n";
    std::cout << "\tUnique words " << filename1 << ": \n";
    std::cout << "\t---------------------------------------------------\n";
    for(const std::string& word : r.uniqueWords[0].words()) {
        std::cout << "\t" << word << "\n";
    }

    std::cout << "\t---------------------------------------------------\n";
    std::cout << "\tUnique words " << filename2 << ": \n";
    std::cout << "\t---------------------------------------------------\n";
    for(const std::string& word : r.uniqueWords[1].words()) {
        std::cout << "\t" << word << "\n";
    }

}

void input_output_handler::say_goodbye() {
    std::cout << "\n\n...Goodbye!\n...press any key to close the program..." << std::endl;
    char ch;
    std::cin.get(ch);
}



