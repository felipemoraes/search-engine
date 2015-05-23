//
//  hit.h
//  search_engine
//
//  Created by Felipe Moraes on 5/23/15.
//
//

#ifndef __search_engine__hit__
#define __search_engine__hit__

#include "doc_repository.h"

class Hit {
public:
    DocumentInfo doc_;
    float score_;
    Hit(DocumentInfo doc, float score);
    
    
    friend bool operator<(const Hit &x, const Hit &y){
        if(x.score_ > y.score_){
            return true;
        } else if (x.score_ < y.score_) {
            return false;
        } else if(x.doc_.pagerank_ > y.doc_.pagerank_) {
            return true;
        } else if(x.doc_.pagerank_ <= y.doc_.pagerank_){
            return false;
        }
        return false;
    }
    
    friend bool operator>(const Hit &x, const Hit &y){
        if(x.score_ < y.score_){
            return true;
        } else if (x.score_ > y.score_) {
            return false;
        } else if(x.doc_.pagerank_ < y.doc_.pagerank_) {
            return true;
        } else if(x.doc_.pagerank_ >= y.doc_.pagerank_){
            return false;
        }
        return false;
    }
    
    
};

#endif /* defined(__search_engine__hit__) */
