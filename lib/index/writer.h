#ifndef WRITER_H_
#define WRITER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include<boost/tokenizer.hpp>
#include "../common/page.h"
#include "../common/term.h"

using namespace std;
using namespace boost;
using namespace std::placeholders;

class Writer {

public:
    Writer(int run_size);
    ~Writer();
    void processPage(Page& p);
    void write (vector<Term>::iterator it);
    int getRunsCounter();
    bool vocabularyContains(string term);
    int getTermIdFromVocabulary(string term);
    int addTermToVocabulary(string term);
    void commit();
    void closeRunFile();
    void openRunFile();
    void removeAccents(string &str);
private:
    
    int run_size_;
    int runs_counter_;
    int length_file_;
    int voc_counter_;
    ofstream temporary_file_;
    map<string,int> vocabulary_;
    vector<Term> buffer;
    
    
    
};


#endif /* WRITER_H_ */