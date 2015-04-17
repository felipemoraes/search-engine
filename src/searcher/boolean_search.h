//
//  boolean_search.h
//  search_engine
//
//  Created by Felipe Moraes on 4/16/15.
//
//

#ifndef __search_engine__boolean_search__
#define __search_engine__boolean_search__

#include <vector>
#include "../../lib/index/index_file.h"
#include <algorithm>

class BoooleanSearch {
public:
    static vector<unsigned> intersection_terms(vector<Term>* &terms);
    static vector<unsigned> union_terms(vector<Term>* &terms);
};


#endif /* defined(__search_engine__boolean_search__) */
