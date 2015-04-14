//
//  main.cc
//  search_engine
//
//  Created by Felipe Moraes on 4/10/15.
//
//


#include <iostream>
#include <algorithm>
#include <string>
#include "../../lib/index/index_file.h"
#include "../../lib/common/vocabulary.h"

using namespace std;


int main(int argc, char** argv){
    
    string input_directory("/Users/felipemoraes/Developer/search-engine/data/");
    
    //Parse comand line arguments
    for(int i=0; i<argc; i++){
        string param(argv[i]);
        if(param == "--directory" || param == "-d"){
            i++;
            input_directory = string(argv[i]);
        }
    }

    IndexFile* index = new IndexFile( input_directory + "index");
    Vocabulary* vocabulary = new Vocabulary(input_directory + "vocabulary");
    

    string word1,word2,op;
    
    while (true) {
        cout << "Search something..." << endl;
        cin >> word1 >> op >> word2;
        int term_id1 = vocabulary->get_term_id(word1);
        long seek1 = vocabulary->get_seek(term_id1);
        int term_id2 = vocabulary->get_term_id(word2);
        long seek2 = vocabulary->get_seek(term_id2);
        Term term1 = index->read(seek1);
        Term term2 = index->read(seek2);
        vector<Doc>::iterator it;
        if (term_id1 == -1 && term_id2 == -1) {
            cout << "Nothing retrieved" << endl;
        } else if (term_id1 == -1) {
            for (it=term2.docs_->begin(); it!=term2.docs_->end(); it++) {
                cout << it->doc_id_ << ' ';
            }
            cout << endl;
        } else if (term_id2 == -1) {
            for (it=term1.docs_->begin(); it!=term1.docs_->end(); it++) {
                cout << it->doc_id_ << ' ';
            }
            cout << endl;
        }
        if (!op.compare("and")){
            vector<int> v1;
            vector<int> v2;
            for (it=term1.docs_->begin(); it!=term1.docs_->end(); it++) {
                v1.push_back( it->doc_id_);
            }
   

            for (it=term2.docs_->begin(); it!=term2.docs_->end(); it++) {
                v2.push_back( it->doc_id_);
            }
            vector<int> result;
            set_intersection(v1.begin(), v1.end(),
                                  v2.begin(), v2.end(),
                                  back_inserter(result));
            
            for (int n : result) {
                cout << n << ' ';
            }
            cout << endl;
        } else if (!op.compare("or")){
            vector<int> v1;
            vector<int> v2;
            for (it=term1.docs_->begin(); it!=term1.docs_->end(); it++) {
                v1.push_back( it->doc_id_);
            }
            
            
            for (it=term2.docs_->begin(); it!=term2.docs_->end(); it++) {
                v2.push_back( it->doc_id_);
            }
            vector<int> result;
            set_union(v1.begin(), v1.end(),
                             v2.begin(), v2.end(),
                             back_inserter(result));
            
            for (int n : result) {
                cout << n << ' ';
            }
            cout << endl;
        }
    }
    delete index;
    return EXIT_SUCCESS;
}