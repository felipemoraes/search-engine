//
//  term.cc
//  search_engine
//
//  Created by Felipe Moraes on 3/28/15.
//
//

#include "term.h"


void Term::setFrequency(){
    frequency_++;
}

void Term::addPosition(int position){
    positions_.push_back(position);
}

string Term::getDocId(){
    return doc_id_;
}

int Term::getFrequency(){
    return frequency_;
}

string Term::getPositions(){
    vector<int>::iterator it;
    string str = "";
    for (it = positions_.begin(); it != positions_.end(); it++){
        str += to_string(*it) + " ";
    }
    return str;
}

Term::Term(int term_id, string doc_id){
    term_id_ = term_id;
    doc_id_ = doc_id;
    frequency_ = 0;
}
string Term::getTerm(){
    return term_;
}

Term::Term(){
    
}

void Term::setTerm(string term){
    term_ = term;
}
int Term::getTermId(){
    return term_id_;
}

Term::Term(string str){
    istringstream iss(str);
    vector<string> tokens{istream_iterator<string>{iss},
        istream_iterator<string>{}};
    vector<string>::iterator it;
    
    term_id_ = atoi(tokens[0].c_str());
    doc_id_ = tokens[1];
    frequency_ = atoi(tokens[2].c_str());
    
    for (int i=3; i < tokens.size(); i++) {
        addPosition(atoi(tokens[i].c_str()));
    }
}
