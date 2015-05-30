//
//  vector_space_model.h
//  search_engine
//
//  Created by Felipe Moraes on 5/16/15.
//
//

#include "ranking_model.h"
#include <unordered_map>
#include <vector>
#include <boost/tokenizer.hpp>
#include "../index/index_file.h"
#include "../common/vocabulary.h"
#include "../common/util.h"
#include "../common/doc_repository.h"
#include "../common/hit.h"


#ifndef __search_engine__vector_space_model__
#define __search_engine__vector_space_model__


class VectorSpaceModel : public RankingModel {
public:

    VectorSpaceModel(IndexFile* index, Vocabulary *vocabulary,  DocRepository* doc_repository);
    vector<Hit>* search(string query);
};


#endif /* defined(__search_engine__vector_space_model__) */
