//
//  title_model.h
//  search_engine
//
//  Created by Felipe Moraes on 5/29/15.
//
//

#ifndef __search_engine__title_model__
#define __search_engine__title_model__


#include "ranking_model.h"
#include <unordered_map>
#include <vector>
#include <boost/tokenizer.hpp>
#include "../index/index_file.h"
#include "../common/vocabulary.h"
#include "../common/util.h"
#include "../common/doc_repository.h"
#include "../common/hit.h"


class TitleModel : public RankingModel {
public:
    
    TitleModel(IndexFile* index, Vocabulary *vocabulary,  DocRepository* doc_repository);
    vector<Hit>* search(string query);
};



#endif /* defined(__search_engine__title_model__) */
