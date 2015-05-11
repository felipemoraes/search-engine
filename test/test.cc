#include <iostream>
#include "../lib/index/index_file.h"
#include <vector>

using namespace std;
int main(){
    IndexFile* index = new IndexFile("/Users/felipemoraes/Developer/search-engine/data/test",2);
    Term term1,term2, term3;
    term1.term_id_ = 1;
    term1.frequency_ = 1;
    term1.docs_ = new vector<Doc>();
    Doc doc1;
    doc1.doc_id_ = 1;
    doc1.frequency_ = 1;
    doc1.positions_ = new vector<unsigned>();
    doc1.positions_->push_back(10);
    term1.docs_->push_back(doc1);
    index->write(term1);
   
    term2.term_id_ = 4;
    term2.frequency_ = 1;
    term2.docs_ = new vector<Doc>();
    Doc doc2;
    doc2.doc_id_ = 1;
    doc2.frequency_ = 1;
    doc2.positions_ = new vector<unsigned>();
    doc2.positions_->push_back(10);
    term2.docs_->push_back(doc2);
    index->write(term2);
    index->close();
    delete index;
    index = new IndexFile("/Users/felipemoraes/Developer/search-engine/data/test");
    term3 = index->read();
    term3 = index->read();
    cout << term3.term_id_ << endl;
    index->close();
    return 0;
}