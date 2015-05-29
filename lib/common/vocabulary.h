//
//  vocabulary.h
//  search_engine
//  Objective: load vocabulary from file and provide efficient get seek and get term identification operations
//  Created by Felipe Moraes on 4/11/15.
//
//

#ifndef __search_engine__vocabulary__
#define __search_engine__vocabulary__

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>


using namespace std;

class Vocabulary {
private:
    unordered_map<string, pair<unsigned, unsigned> >* terms;
    unordered_map<int, long >* seeks;
public:
    
    Vocabulary(string filename);
    ~Vocabulary();
    int get_term_id(string term);
    long get_seek(int term_id);
    int get_frequence(string term);
};


#endif /* defined(__search_engine__vocabulary__) */
