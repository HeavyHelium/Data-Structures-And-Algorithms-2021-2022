#include "data_parser.h"

std::ostream& operator<<(std::ostream& os,
                         const manager_subordinate& p)
{
    return os << p.manager
              << "-"
              << p.subordinate;
}

std::string data_parser::console_data()
{
    std::string text;
    std::string line;
    while(std::getline(std::cin, line)) {
        text += line;
        text.push_back('\n');
    }
    std::cin.clear();
    return text;
}

std::string data_parser::file_data(const std::string& path) { return file_to_string(path); }

void data_parser::parse_text(const std::string& text)
{
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

const std::vector<manager_subordinate>& data_parser::pairs()
{ return ms_pairs; }

void data_parser::clear_states()
{
    line = 1;
    ms_pairs.clear();
}

std::vector<std::string> data_parser::parse_line(const char*& text)
{
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
    {
        if(*text == '-')
            throw std::logic_error("wrong input format");
        word.push_back(*text++);
    }
    skip_spaces_and_tabs(text);
    if(word.empty() || *text && *text != '\n')
        throw std::logic_error("wrong input format");
    words.push_back(word);
    return words;
}

std::string data_parser::file_to_string(const std::string& path)
{
    std::ifstream ifile(path.c_str(), std::ios_base::in);
    if(!ifile)
        throw std::runtime_error("failed to open input file with path \"" +
                                 path + '\"');
    std::string line, text;
    while(std::getline(ifile, line)) text += line + '\n';
    return text;
}
