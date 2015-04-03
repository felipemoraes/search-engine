//
//  term_occurrence.cc
//  search_engine
//
//  Created by Felipe Moraes on 3/28/15.
//
//

#include "term_occurrence.h"


TermOccurrence::TermOccurrence(int term_id, int doc_id, int frequency){
    term_id_ = term_id;
    doc_id_ = doc_id;
    frequency_ = frequency;
}

TermOccurrence::TermOccurrence(){
    
}


void TermOccurrence::add_position(int position){
    positions_.push_back(position);
}


string TermOccurrence::get_positions(){
    vector<int>::iterator it;
    string str = "";
    for (it = positions_.begin(); it != positions_.end(); it++){
        str += to_string(*it) + " ";
    }
    return str;
}



