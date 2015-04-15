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
#include <unordered_set>
#include<boost/tokenizer.hpp>
#include "../common/page.h"
#include "../common/term_occurrence.h"
#include "../common/file.h"

using namespace std;
using namespace boost;
using namespace std::placeholders;


class Mapper {

public:
    Mapper(unsigned run_size, string index_directory, string stopwords_directory);
    ~Mapper();
    void process_page(Page& p);
    void write (vector<TermOccurrence>::iterator it);
    int add_vocabulary(string term);
    void remove_accents(string &str);
    void process_frequencies(Page& p, unordered_map<string, vector<unsigned> > &positions);
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
    unordered_set<string> stopwords_;
    vector<File* >* runs_;
    ofstream doc_file_;
    void load_stopwords(string stopwords_dir);
    
};


#endif /* defined(__search_engine__run__) */
