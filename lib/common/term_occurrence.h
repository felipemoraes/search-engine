//
//  term_occurrence.h
//  search_engine
//
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
private:
    vector<unsigned> positions_;
public:
    TermOccurrence(unsigned term_id, unsigned doc_id, vector<unsigned> positions);
    
    TermOccurrence();
    ~TermOccurrence();
    
    
    unsigned term_id_;
    unsigned doc_id_;
    unsigned frequency_;
    unsigned run_number_;
    
    void add_position(unsigned position);
    vector<unsigned> get_positions();
    
    
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
