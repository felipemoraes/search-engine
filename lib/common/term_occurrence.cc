//
//  term_occurrence.cc
//  search_engine
//  Objective: term occurence data structure
//  Created by Felipe Moraes on 3/28/15.
//
//

#include "term_occurrence.h"


TermOccurrence::TermOccurrence(unsigned term_id, unsigned doc_id, unsigned frequency, unsigned field){
    term_id_ = term_id;
    doc_id_ = doc_id;
    frequency_ = frequency;
    field_ = field;
}

TermOccurrence::TermOccurrence(){
}


TermOccurrence::~TermOccurrence(){
    
}



