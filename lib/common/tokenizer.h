#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <string>
using namespace std;

class Tokenizer {
    
    
    
    string text_;
    unsigned int token_start_;
    unsigned int current_char_;
    
    bool isSpecialChar(char& ch);
    bool isAlphaNum(char& ch);
    
public:
    Tokenizer(string&);
    ~Tokenizer();
    
    bool hasNext();
    string nextToken();
    void removeAccents(string &str);
};

#endif /* TEXTTOKENIZER_H_ */