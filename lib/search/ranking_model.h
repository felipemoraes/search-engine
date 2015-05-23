//
//  ranking_model.h
//  search_engine
//
//  Created by Felipe Moraes on 5/16/15.
//
//

#ifndef __search_engine__ranking_model__
#define __search_engine__ranking_model__

#include <unordered_map>
#include "../index/index_file.h"
#include "../common/vocabulary.h"
#include "../common/doc_repository.h"
#include "../common/hit.h"
using namespace std;

class RankingModel {
public:
    RankingModel(IndexFile* index, Vocabulary *vocabulary, DocRepository* doc_repository);
    IndexFile* index_;
    Vocabulary *vocabulary_;
    DocRepository* doc_repository_;
    virtual vector<Hit>* search(string query) = 0;
};

#endif /* defined(__search_engine__ranking_model__) */
