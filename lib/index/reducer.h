//
//  reducer.h
//  search_engine
//  Objective: reduce file in one and aggregate it in a index file
//  Created by Felipe Moraes on 3/28/15.
//
//

#ifndef __search_engine__reducer__
#define __search_engine__reducer__

#include "../common/term_occurrence.h"
#include "../common/file.h"
#include "../index/index_file.h"
#include <vector>
#include <unordered_map>

class Reducer {
private:
    unsigned buffer_size_;
    vector<File* >* runs_;
    File* merged_;
    unsigned block_number_;
    string directory_;
public:
    Reducer(unsigned buffer_size, vector<File* >* &runs, string index_directory);
    ~Reducer();
    void merge();
    File* kmerge(vector<File* >* &runs);
    pair<vector<long>*,vector<long>*> reduce(unsigned voc_size, unsigned anchor_size);
    void aggregate(IndexFile &index, Term& aggr, TermOccurrence term, unsigned& previous);
};


#endif /* defined(__search_engine__reducer__) */
