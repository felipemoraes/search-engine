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
 #include <boost/algorithm/string.hpp>
#include "../common/page.h"
#include "../common/term_occurrence.h"
#include "../common/file.h"
#include "../common/util.h"
#include "../common/graph.h"
#include "../common/doc_repository.h"

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
    void process_frequencies(string text, map<string, unsigned > &frequencies);
    void add_buffer(unsigned term_id, unsigned doc_id, unsigned frequency,unsigned field);
    void flush();
    vector<File* >* exec();
    vector<File* >* get_runs();
    void dump(unordered_map<unsigned,long>* &seeks_voc, unordered_map<unsigned,long>* &seeks_anchor);
    int get_vocabulary_size();
    int get_vocabulary_anchor_size();
    unordered_map<string, unsigned >* get_urls_anchor();
    unordered_map<string,unsigned >* get_urls();
    DocRepository* get_docs_anchor();
    void remove_doc_anchor(string doc_url);
    void remove_s(string &s);
private:
    
    unsigned run_size_;
    unsigned buffer_size_;
    unsigned voc_counter_;
    unsigned voc_anchor_counter_;
    unsigned doc_counter_;
    unsigned doc_counter_anchor_;
    string directory_;
    unordered_map<string,pair<unsigned,unsigned> >* vocabulary_;
    unordered_map<string,pair<unsigned,unsigned> >* vocabulary_anchor_;
    DocRepository* docs_;
    unordered_map<string,unsigned>* urls_;
    unordered_map<string,unsigned>* urls_anchor_;
    DocRepository* docs_anchor_;
    unordered_map<unsigned,vector<unsigned> >* links_;
    vector<float>* pagerank_;
    vector<TermOccurrence>* buffer;
    unordered_set<string> stopwords_;
    vector<File* >* runs_;
    fstream outlinks_file_;
    void process_pagerank(int size);
};


#endif /* defined(__search_engine__run__) */
