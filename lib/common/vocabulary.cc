//
//  vocabulary.cc
//  search_engine
//  Objective: load vocabulary from file and provide efficient get seek and get term identification operations
//  Created by Felipe Moraes on 4/11/15.
//
//

#include "vocabulary.h"


Vocabulary::Vocabulary(string filename){
    ifstream file;
    file.open(filename);
    string term; unsigned term_id; unsigned frequence; long seek;
    terms = new unordered_map<string, pair<unsigned, unsigned>>();
    seeks = new unordered_map<int, long>();
    while (!file.eof()) {
        file >> term >> term_id >> frequence >> seek;
        terms->insert(make_pair(term, make_pair(term_id, frequence)));
        seeks->insert(make_pair(term_id, seek));
    }
}

Vocabulary::~Vocabulary(){
    delete terms;
    delete seeks;
}

long Vocabulary::get_seek(int term_id){
    if (seeks->find(term_id) != seeks->end()) {
        return (*seeks)[term_id];
    } else {
        return -1;
    }
}

int Vocabulary::get_term_id(string term){
    if (terms->find(term)!= terms->end()) {
        return (*terms)[term].first;
    } else {
        return -1;
    }
}

int Vocabulary::get_frequence(string term){
    if (terms->find(term)!=terms->end()) {
        return (*terms)[term].second;
    } else {
        return -1;
    }
}