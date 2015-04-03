//
//  merge.h
//  search_engine
//
//  Created by Felipe Moraes on 3/28/15.
//
//

#ifndef __search_engine__merge__
#define __search_engine__merge__

#include "../common/term_occurrence.h"
#include "../common/file.h"
#include <vector>

class Merge {
private:
    int buffer_size_;
    vector<File<TermOccurrence>* > runs_;
public:
    Merge(int buffer_size, vector<File<TermOccurrence>* > &runs);
    File<TermOccurrence>* merge_runs();

    
};


#endif /* defined(__search_engine__merge__) */
