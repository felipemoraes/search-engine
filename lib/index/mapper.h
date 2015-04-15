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
#include <unordered_map>
#include<boost/tokenizer.hpp>
#include "../common/page.h"
#include "../common/term_occurrence.h"
#include "../common/file.h"

using namespace std;
using namespace boost;
using namespace std::placeholders;

class Mapper {

public:
    Mapper(unsigned run_size, string index_directory);
    ~Mapper();
    void process_page(Page& p);
    void write (vector<TermOccurrence>::iterator it);
    bool vocabulary_contains(string term);
    int add_vocabulary(string term);
    void remove_accents(string &str);
    void process_frequencies(Page& p, map<string, vector<unsigned> > &positions);
    void add_buffer(unsigned term_id, unsigned doc_id, vector<unsigned> positions);
    void flush();
    vector<File* >* exec();
    vector<File* >* get_runs();
    void dump(vector<long>* &seeks);
    
    int get_vocabulary_size();
    
private:
    
    unsigned run_size_;
    unsigned buffer_size_;
    unsigned voc_counter_;
    unsigned doc_counter_;
    string directory_;
    unordered_map<string,unsigned>* vocabulary_;
    vector<TermOccurrence>* buffer;
    vector<File* >* runs_;
    ofstream doc_file_;
    
    
    
};


#endif /* defined(__search_engine__run__) */
