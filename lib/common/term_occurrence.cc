//
//  term_occurrence.cc
//  search_engine
//
//  Created by Felipe Moraes on 3/28/15.
//
//

#include "term_occurrence.h"


TermOccurrence::TermOccurrence(int term_id, int doc_id, vector<int> positions){
    term_id_ = term_id;
    doc_id_ = doc_id;
    positions_ = new vector<int>(positions);
    frequency_ = positions_->size();
}

TermOccurrence::TermOccurrence(){
    positions_ = new vector<int>();
}


void TermOccurrence::add_position(int position){
    positions_->push_back(position);
}


vector<int>* TermOccurrence::get_positions(){
    return positions_;
}



