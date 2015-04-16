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
    string term; int term_id; long seek;
    term_ids = new map<string, int>();
    seeks = new map<int, long>();
    while (!file.eof()) {
        file >> term >> term_id >> seek;
        term_ids->insert(make_pair(term, term_id));
        seeks->insert(make_pair(term_id, seek));
    }
}

long Vocabulary::get_seek(int term_id){
    if (seeks->count(term_id)) {
        return (*seeks)[term_id];
    } else {
        return -1;
    }
}

int Vocabulary::get_term_id(string term){
    if (term_ids->count(term)) {
        return (*term_ids)[term];
    } else {
        return -1;
    }
}