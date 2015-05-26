//
//  model_combiner.cpp
//  search_engine
//
//  Created by Felipe Moraes on 5/24/15.
//
//

#include "model_combiner.h"

vector<Hit>* ModelCombiner::linear_combiner(vector<RankingModel*> models, string query){
    unordered_map<unsigned, pair<float, DocumentInfo> >* accumulators = new unordered_map<unsigned,  pair<float, DocumentInfo>>();
    vector<Hit>* hits = new vector<Hit>();
    for (auto i = 0; i < models.size(); ++i) {
        vector<Hit>* results = (*models[i]).search(query);
        for (auto hit: *results) {
            auto it = accumulators->find(hit.doc_.doc_id_);
            if (it != accumulators->end()) {
                (*accumulators)[hit.doc_.doc_id_].first += hit.score_*(*models[i]).weight_;
            } else {
                (*accumulators)[hit.doc_.doc_id_].first = hit.score_*(*models[i]).weight_;
                (*accumulators)[hit.doc_.doc_id_].second = hit.doc_;
            }
        }
        delete results;
    }
 
    for (auto &result: *accumulators) {
        Hit hit(result.second.second, result.first);
        hit.score_ = result.second.first;
        hits->push_back(hit);
    }
    delete accumulators;
    sort(hits->begin(), hits->end());
    return hits;
}