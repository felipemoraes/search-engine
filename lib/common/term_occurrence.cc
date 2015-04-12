//
//  term_occurrence.cc
//  search_engine
//
//  Created by Felipe Moraes on 3/28/15.
//
//

#include "term_occurrence.h"


TermOccurrence::TermOccurrence(unsigned term_id, unsigned doc_id, vector<unsigned> positions){
    term_id_ = term_id;
    doc_id_ = doc_id;
    positions_ = new vector<unsigned>(positions);
    frequency_ = positions_->size();
}

TermOccurrence::TermOccurrence(){
    positions_ = new vector<unsigned>();
}


void TermOccurrence::add_position(unsigned position){
    positions_->push_back(position);
}


vector<unsigned>* TermOccurrence::get_positions(){
    return positions_;
}



