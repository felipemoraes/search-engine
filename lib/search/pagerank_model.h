//
//  pagerank_model.h
//  search_engine
//
//  Created by Felipe Moraes on 5/16/15.
//
//


#include "ranking_model.h"
#include <boost/tokenizer.hpp>
#include "../common/util.h"
#include "../common/hit.h"
#include <unordered_map>

#ifndef __search_engine__pagerank_model__
#define __search_engine__pagerank_model__

class PageRankModel : RankingModel {
public:
    PageRankModel(IndexFile* index, Vocabulary *vocabulary, DocRepository* doc_repository);
    vector<Hit>* search(string query);
};



#endif /* defined(__search_engine__pagerank_model__) */
