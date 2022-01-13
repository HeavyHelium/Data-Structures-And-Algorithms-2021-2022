#include <iostream>
#include <cmath>
#include "input_output_handler.h"
#include "interface.h"
//#define SAVE_RESULTS

void input_output_handler::handle_input(std::ifstream& ifile1, std::ifstream& ifile2) {
    Comparator c;
    ComparisonReport report = c.compare(ifile1, ifile2);

    ifile1.close();
    ifile2.close();

    total_common = report.commonWords.total_word_cnt();
    fst_unique = report.uniqueWords[0].total_word_cnt();
    fst_total  = total_common + fst_unique;
    snd_unique = report.uniqueWords[1].total_word_cnt();
    snd_total  = total_common + snd_unique;

    similarity_rate1 = total_common * 100.0 / fst_total;
    similarity_rate2 = total_common * 100.0 / snd_total;
#ifdef SAVE_RESULTS
    save_as_hashed_and_sorted(report);
#endif

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
              << " %.\n";
}
void input_output_handler::say_goodbye() {
    std::cout << "\n\n...Goodbye!\n...press any key to close the program..." << std::endl;
    char ch;
    std::cin.get(ch);
}

void input_output_handler::save_as_hashed_and_sorted(const ComparisonReport& rep,
                                                     const char* filepath1,
                                                     const char* filepath2,
                                                     const char* filepath3) {
    std::ofstream f1(filepath1);
    std::ofstream f2(filepath2);
    std::ofstream f3(filepath3);
    const auto& uset1 = rep.uniqueWords[0].words();
    const auto& uset2 = rep.uniqueWords[1].words();
    const auto& common = rep.commonWords.words();
    for(const auto& word : uset1)
        f1 << word << '\n';
    for(const auto& word : uset2)
        f2 << word << '\n';
    for(const auto& word : common)
        f3 << word << '\n';
}
