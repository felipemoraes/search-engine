//
//  term_occurrence.cc
//  search_engine
//  Objective: term occurence data structure
//  Created by Felipe Moraes on 3/28/15.
//
//

#include "term_occurrence.h"


TermOccurrence::TermOccurrence(unsigned term_id, unsigned doc_id, vector<unsigned> positions, unsigned field){
    term_id_ = term_id;
    doc_id_ = doc_id;
    positions_ = positions;
    frequency_ = positions_.size();
    field_ = field;
}

TermOccurrence::TermOccurrence(){
}


TermOccurrence::~TermOccurrence(){
    
}


void TermOccurrence::add_position(unsigned position){
    positions_.push_back(position);
}


vector<unsigned> TermOccurrence::get_positions(){
    return positions_;
}



