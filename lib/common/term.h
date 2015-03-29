//
//  term.h
//  search_engine
//
//  Created by Felipe Moraes on 3/28/15.
//
//

#ifndef __search_engine__term__
#define __search_engine__term__

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Term {
private:
    int frequency_;
    string term_;
    string doc_id_;
    vector<int> positions_;
public:
    Term(int term_id, string doc_id);
    Term();
    Term(string str);
    void setFrequency();
    void addPosition(int position);
    string getDocId();
    int getFrequency();
    string getPositions();
    string getTerm();
    int term_id_;
    int getTermId();
    void setTerm(string term);
    
};

#endif /* defined(__search_engine__term__) */
