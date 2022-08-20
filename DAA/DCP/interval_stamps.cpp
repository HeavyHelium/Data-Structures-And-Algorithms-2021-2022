#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <cstring>
#include <typeinfo>
#include <stdexcept>
#include <map>
#include <algorithm>
#include <utility>


static bool is_whitespace(const char ch) { 
    return ch == ' '  ||
           ch == '\t' || 
           ch == '\n';
}


void skip_whitespace(const char*& p) {
    while(*p && is_whitespace(*p)) {
        ++p;
    }
}


struct stamp_parser { 

    static const inline std::vector<std::string> lables{ "\"timestamp\"", 
                                                         "count", 
                                                         "\"type\"" };

    stamp_parser(const char* line) { 
        int cid = 0;
        if(*line++ != '{') { 
            throw std::invalid_argument("bad stamp");
        } 

        time = view_to_int(parse_entry(line, lables[cid++]));
        cnt = view_to_int(parse_entry(line, lables[cid++]));
        std::string_view type = parse_entry(line, lables[cid++]);
        
        if(type == "\"enter\"") { 
        } else if(type == "\"exit\"") { 
            cnt *= -1;
        } else { 
            throw std::invalid_argument("bad stamp");
        }
        if(*line) { 
            throw std::invalid_argument("bad stamp");    
        }
    }

    int timestamp() const { 
        return time;
    }
    int value() const { 
        return cnt;
    }

private: 
    std::string_view parse_entry(const char*& line, 
                                 const std::string& label) {
        skip_whitespace(line); 

        std::string_view word1 = extract_word(line);
        if(word1 != label) {
            throw std::runtime_error("bad entry");
        }
        skip_whitespace(line);
        
        if(*line++ != ':') { 
            throw std::runtime_error("bad entry");
        }
        skip_whitespace(line);

        std::string_view word3 = extract_word(line);
        if(word3.empty()) { 
            throw std::runtime_error("expected data for entry " + label);    
        }
        skip_whitespace(line);
        if(*line != ',') { 
            if(*line == '}') { 
                ++line;
                skip_whitespace(line);
                if(*line) { 
                    throw std::runtime_error("bad entry");
                }
            } else { 
                throw std::runtime_error("bad entry");
            }
        } else {
            ++line;
        } 
        return word3;
    }

    std::string_view extract_word(const char*& line) { 
        skip_whitespace(line);
        const char* begin = line;
        while(*line && 
              !is_whitespace(*line) && 
              *line != ':' && 
              *line != ',' && *line != '}') { 
            ++line;
        }
        return { begin, line - begin };
    }
public:
    static bool is_digit(char ch) { 
        return ch <= '9' && ch >= '0';
    }

    static int view_to_int(const std::string_view& s) { 
        auto iter = s.begin();
        int sign = 1;
        
        if(*iter == '-') { 
            sign = -1;
            ++iter;
        }
        
        int val = 0;
        while(iter != s.end()) {
            if(!is_digit(*iter)) { 
                throw std::invalid_argument("argument is not an integer");
            } else { 
                val *= 10;
                val += (*iter - '0'); 
            }
            ++iter;
        }
        return val * sign;
    }
private:
    int time = -1;
    int cnt = 0;
};

struct solution { 
    using const_iter = std::map<int, int>::const_iterator; 
 
    friend std::istream& operator>>(std::istream& is, 
                                    solution& s) { 
        std::string line;
        while(std::getline(is, line)) { 
            const char* l = line.c_str();
            skip_whitespace(l);
            if(!*l) { 
                continue;
            }
            stamp_parser p(l);
            s.tree.emplace(p.timestamp(), p.value());   
        }
        return is;
    }

    void solve() {
        std::map<int, int>::const_iterator iter = tree.begin();   
        if(iter == tree.end()) {
            return;
        }
        int so_far = iter++->second; 
        int max = so_far;
        
        const_iter max_beg = tree.begin();
        const_iter max_end = max_beg;

        const_iter curr_beg = max_beg;
        const_iter curr_end = max_beg;


        while(iter != tree.end()) { 
            if(so_far >= 0) { 
                so_far += iter->second;
                curr_end = iter;
            } else { 
                so_far = iter->second;
                curr_beg = curr_end = iter;
            }        

            if(so_far > max) { 
                max = so_far;
                max_beg = curr_beg;
                max_end = curr_end;
            }
            ++iter;
        }

        std::cout << "Max number of clients in: " << max << "\n";
        std::cout << "Timestamp period: ";
        std::cout << "(" << max_beg->first 
                  << ", " << max_end->first 
                  << ")\n";
            
    }

private: 
    std::map<int, int> tree;    
};


int main() try { 

    const char* s = "{\"timestamp\": 1, count: 10, \"type\": \"exit\"}";

    solution sol;
    std::cin >> sol;    
    sol.solve();

    return 0;

} catch(const std::exception& e) { 
    std::cout << typeid(e).name() << ": " << std::endl;
    std::cerr << e.what() << "\n";
    return -1;
}