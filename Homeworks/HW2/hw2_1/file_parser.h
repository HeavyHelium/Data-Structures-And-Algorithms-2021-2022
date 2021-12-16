#ifndef HW2_1_FILE_PARSER
#define HW2_1_FILE_PARSER
#include <vector>
#include <string>
#include <fstream>
#include <cstddef>
#include <cassert>
#include "string_operations.h"

struct manager_subordinate
{
    std::string manager = "";
    std::string subordinate = "";
    friend std::ostream& operator<<(std::ostream& os,
                                    const manager_subordinate& p)
    {
       return os << p.manager
                 << "-"
                 << p.subordinate;
    }
    friend std::istream& operator>>(std::istream& is,
                                    manager_subordinate& p)
    {
        std::string str;
        if(!is) return is;
        while(is_line_empty(str) && std::getline(is, str));
        const char* line = str.c_str();
        skip_white_space(line);
        std::vector<std::string> words;
        std::string word;
        while(*line && !is_white_space(*line) && *line != '-')
            word.push_back(*line++);
        skip_white_space(line);
        if(word.empty() || *line != '-')
            throw std::logic_error("wrong input format");
        ++line;
        skip_white_space(line);
        words.push_back(word);
        word.clear();
        while(*line && !is_white_space(*line))
            word.push_back(*line++);
        skip_white_space(line);
        if(word.empty() || *line)
            throw std::logic_error("wrong input format");
        words.push_back(word);
        assert(words.size() == 2);
        p.manager = words[0];
        p.subordinate = words[1];
        return is;
    }
};

class data_parser
{
    std::vector<manager_subordinate> ms_pairs;
    std::size_t line = 1;
public:
    data_parser() = default;
    static std::string file_to_string(const std::string& path)
    {
        std::ifstream ifile(path.c_str(), std::ios_base::in);
        if(!ifile)
            throw std::runtime_error("failed to open input file with path \"" +
                                     path + '\"');
        std::string line, text;
        while(std::getline(ifile, line)) text += line + '\n';
        return text;
    }
    static std::string console_input_to_string()
    {
        std::string line, text;
        while(std::getline(std::cin, line)) text += line + '\n';
        return text;
    }
    void parse_text(const std::string& text)
    {
        //std::cout << text << std::endl;
        const char* str = text.c_str();
        while(*str)
        {
            if(*str == '\n') { ++line; ++str;}
            else
            {
                std::vector<std::string> words;
                try
                { words = parse_line(str); }
                catch(const std::exception& e)
                {
                    throw std::runtime_error("on line: " +
                                             std::to_string(line) +
                                             ": " + e.what());
                }
                if(!words.empty())
                    ms_pairs.push_back(manager_subordinate{ words[0], words[1] });
            }
        }
    }
    static std::vector<std::string> parse_line(const char*& text)
    {
        //std::cout << text << std::endl;
        skip_spaces_and_tabs(text);
        std::vector<std::string> words;
        if(!*text || *text == '\n') { return words; }
        std::string word;
        while(*text && !is_white_space(*text) && *text != '-')
            word.push_back(*text++);
        skip_spaces_and_tabs(text);
        if(word.empty() || *text != '-')
        { throw std::logic_error("wrong input format"); }
        ++text;
        skip_spaces_and_tabs(text);
        words.push_back(word);
        word.clear();
        while(*text && !is_white_space(*text))
            word.push_back(*text++);
        skip_spaces_and_tabs(text);
        if(word.empty() || *text && *text != '\n')
        {
            //std::cout << word << std::endl;
            throw std::logic_error("wrong input format");
        }
        words.push_back(word);
        return words;
    }
    const std::vector<manager_subordinate>& pairs() { return ms_pairs; }
private:
};

#endif //HW2_1_FILE_PARSER

//
//
//        std::size_t line_num = 0;
//        std::string line;
//        try {
//            while(std::getline(ifile, line) && ++line_num)
//            {
//                if(is_line_empty(line)) continue;
//                std::vector<std::string> words = parse_line(line.c_str());
//                ms_pairs.push_back(manager_subordinate{ words[0], words[1] });
//            }
//        }
//        catch(const std::logic_error& e)
//        {
//            throw std::runtime_error("on line: " +
//                                      std::to_string(line_num) +
//                                      ": " + e.what());
//        }
