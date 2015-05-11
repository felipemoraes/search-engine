//
//  mapper.h
//  search_engine
//  Objective: process texts and write it in sorted files
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
    bool vocabulary_contains(string term);
    int add_vocabulary(const string& term);
    int add_vocabulary_anchor(const string& term);
    void remove_accents(string &str);
    void process_frequencies(string text, map<string, vector<unsigned> > &positions);
    void add_buffer(unsigned term_id, unsigned doc_id, vector<unsigned> positions,unsigned field);
    void flush();
    vector<File* >* exec();
    vector<File* >* get_runs();
    void dump(vector<long>* &seeks_voc, vector<long>* &seeks_anchor);
    int get_vocabulary_size();
    int get_vocabulary_anchor_size();
    
private:
    
    unsigned run_size_;
    unsigned buffer_size_;
    unsigned voc_counter_;
    unsigned voc_anchor_counter_;
    unsigned doc_counter_;
    string directory_;
    unordered_map<string,unsigned>* vocabulary_;
    unordered_map<string,unsigned>* vocabulary_anchor_;
    unordered_map<string,pair<unsigned,unsigned> >* docs_;
    unordered_map<unsigned,vector<unsigned> >* links_;
    vector<TermOccurrence>* buffer;
    unordered_set<string> stopwords_;
    vector<File* >* runs_;
    ofstream doc_file_;
    void load_stopwords(string stopwords_dir);
    
};


#endif /* defined(__search_engine__run__) */
