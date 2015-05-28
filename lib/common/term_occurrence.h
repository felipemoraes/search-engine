//
//  term_occurrence.h
//  search_engine
//  Objective: term occurence data structure
//  Created by Felipe Moraes on 3/28/15.
//
//

#ifndef __search_engine__term_occurrence__
#define __search_engine__term_occurrence__

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class TermOccurrence {
public:
    TermOccurrence(unsigned term_id, unsigned doc_id, unsigned frequency, unsigned field);
    
    TermOccurrence();
    ~TermOccurrence();
    
    
    unsigned term_id_;
    unsigned doc_id_;
    unsigned frequency_;
    unsigned run_number_;
    unsigned field_;
    
    
    friend bool operator>(const TermOccurrence &x, const TermOccurrence &y){
        if(x.term_id_ > y.term_id_){
            return true;
        } else if (x.term_id_ < y.term_id_) {
            return false;
        } else if(x.doc_id_ > y.doc_id_) {
            return true;
        } else if(x.doc_id_ <= y.doc_id_){
            return false;
        }
        return false;
    }
    
    friend bool operator<(const TermOccurrence &x, const TermOccurrence &y){
        if(x.term_id_ < y.term_id_){
            return true;
        } else if (x.term_id_ > y.term_id_) {
            return false;
        } else if(x.doc_id_ < y.doc_id_) {
            return true;
        } else if(x.doc_id_ >= y.doc_id_){
            return false;
        }
        return false;
    }

};


#endif /* defined(__search_engine__term_occorrence__) */
