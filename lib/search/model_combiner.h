//
//  model_combiner.h
//  search_engine
//
//  Created by Felipe Moraes on 5/24/15.
//
//

#ifndef __search_engine__model_combiner__
#define __search_engine__model_combiner__

#include "ranking_model.h"
#include "../common/hit.h"
#include "../common/doc_repository.h"
#include <vector>
#include <unordered_map>

using namespace std;

class ModelCombiner {
public:
    vector<Hit>* linear_combiner(vector<RankingModel*> models, string query);
};

#endif /* defined(__search_engine__model_combiner__) */
