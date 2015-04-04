//
//  reducer.h
//  search_engine
//
//  Created by Felipe Moraes on 3/28/15.
//
//

#ifndef __search_engine__reducer__
#define __search_engine__reducer__

#include "../common/term_occurrence.h"
#include "../common/file.h"
#include "../index/index_file.h"
#include <vector>

class Reducer {
private:
    int buffer_size_;
    vector<File* > runs_;
    File* merged_;
public:
    Reducer(int buffer_size, vector<File* > &runs);
    void merge();
    void reduce();

    
};


#endif /* defined(__search_engine__reducer__) */
