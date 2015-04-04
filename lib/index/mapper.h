//
//  mapper.h
//  search_engine
//
//  Created by Felipe Moraes on 3/28/15.
//
//


#ifndef __search_engine__writer__
#define __search_engine__writer__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include<boost/tokenizer.hpp>
#include "../common/page.h"
#include "../common/term_occurrence.h"
#include "../common/file.h"

using namespace std;
using namespace boost;
using namespace std::placeholders;

class Mapper {

public:
    Mapper(int run_size);
    ~Mapper();
    void process_page(Page& p);
    void write (vector<TermOccurrence>::iterator it);
    bool vocabulary_contains(string term);
    int add_vocabulary(string term);
    void remove_accents(string &str);
    void process_frequencies(Page& p,map<string, vector<int> > &positions);
    void add_buffer(int term_id, int doc_id, vector<int> positions);
    void flush();
    vector<File* > exec();
    vector<File* > get_runs();
    
private:
    
    int run_size_;
    int buffer_size_;
    int voc_counter_;
    int doc_counter_;
    map<string,int> vocabulary_;
    TermOccurrence* buffer;
    vector<File* > runs_;
    
    
    
};


#endif /* defined(__search_engine__run__) */