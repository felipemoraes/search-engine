//
//  doc_repository.h
//  search_engine
//
//  Created by Felipe Moraes on 5/19/15.
//
//

#ifndef __search_engine__doc_repository__
#define __search_engine__doc_repository__

#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;


class DocumentInfo {
public:
    unsigned doc_id_;
    string url_;
    string title_;
    float pagerank_;
    unsigned length_;
};

class DocRepository {
private:
    string name_;
    unordered_map<unsigned, DocumentInfo>* documents_;
    fstream file_;
public:
    unsigned size_;
    DocRepository(string name);
    ~DocRepository();
    void insert(DocumentInfo doc);
    DocumentInfo find(unsigned doc_id);
    void remove(unsigned doc_id);
    void dump(string directory);
    void load(string directory);
};
#endif /* defined(__search_engine__doc_repository__) */
