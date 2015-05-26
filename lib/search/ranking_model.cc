//
//  ranking_model.cc
//  search_engine
//
//  Created by Felipe Moraes on 5/16/15.
//
//

#include "ranking_model.h"

RankingModel::RankingModel(IndexFile* index, Vocabulary *vocabulary, DocRepository* doc_repository){
    index_ = index;
    vocabulary_ = vocabulary;
    doc_repository_ = doc_repository;
    weight_ = 1;
}

