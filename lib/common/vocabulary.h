//
//  vocabulary.h
//  search_engine
//
//  Created by Felipe Moraes on 4/11/15.
//
//

#ifndef __search_engine__vocabulary__
#define __search_engine__vocabulary__

#include <iostream>
#include <fstream>
#include <string>
#include <map>


using namespace std;

class Vocabulary {
private:
    map<string, int >* term_ids;
    map<int, long >* seeks;
public:
    
    Vocabulary(string filename);
    int get_term_id(string term);
    long get_seek(int term_id);
};


#endif /* defined(__search_engine__vocabulary__) */
