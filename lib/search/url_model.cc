//
//  url_model.cpp
//  search_engine
//
//  Created by Felipe Moraes on 5/29/15.
//
//

#include "url_model.h"


//
//  title_model.cc
//  search_engine
//
//  Created by Felipe Moraes on 5/29/15.
//
//

#include "title_model.h"


URLModel::URLModel(IndexFile* index, Vocabulary *vocabulary, DocRepository* doc_repository)  : RankingModel(index,vocabulary,doc_repository) {
    name_ = "url";
}




vector<Hit>* URLModel::search(string query){
    
    unordered_map<unsigned,float>* accumulators = new unordered_map<unsigned,float>();
    remove_accents(query);
    transform(query.begin(), query.end(), query.begin(),::tolower);
    tokenizer<> tokens(query);
    // aggregate terms by positions
    
    string term_concate = "";
    for(auto token = tokens.begin(); token!=tokens.end();++token){
        term_concate += *token;
    }
    for(auto token = tokens.begin(); token!=tokens.end();++token){
        // check if term is not a stopword
        // if (stopwords_.find(*token) != stopwords_.end()) {
        //     continue;
        // }
        string t = *token;
        clean_token(t);
        
        int term_id = vocabulary_->get_term_id(t);
        
        long seek = vocabulary_->get_seek(term_id);
        Term term = index_->read(seek);
        
        
        if (term_id != -1) {
            
            for (auto it = term.docs_->begin(); it != term.docs_->end();++it) {
                
                DocumentInfo doc = doc_repository_->find(it->doc_id_);
                if (doc.doc_id_ == it->doc_id_ + 1) {
                    continue;
                }
                if (accumulators->find(it->doc_id_) == accumulators->end()) {
                    accumulators->insert(make_pair(it->doc_id_, 0));
                }
                string url = doc.url_;
                if (url.find(term_concate)) {
                    (*accumulators)[it->doc_id_] = 1;
                }
            }
            
        }
        delete term.docs_;
    }
    vector<Hit>* hits = new vector<Hit>();
    for (auto item: *accumulators) {
        Hit hit(doc_repository_->find(item.first),item.second);
        hits->push_back(hit);
    }
    normalize(*hits);
    sort(hits->begin(),hits->end());
    delete accumulators;
    return hits;
}