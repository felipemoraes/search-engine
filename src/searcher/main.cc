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

#include <sys/time.h>
#include <sys/resource.h>
#include "../../lib/index/index_file.h"
#include "../../lib/common/vocabulary.h"

using namespace std;

static vector<unsigned> intersection_terms(vector<Term>* &terms){
    vector<unsigned> result;
    vector<unsigned> v1;
    vector<unsigned> v2;
    result.clear();
    v1.clear();
    v2.clear();
    if (terms->size() == 0) {
        cout << "Empty result. Try other query." << endl;
        return result;
    }
    Term term = terms->back();
    for (auto it = term.docs_->begin(); it != term.docs_->end(); it++) {
        v1.push_back(it->doc_id_);
    }
    terms->pop_back();
    
    for (auto it = terms->begin(); it!=terms->end(); it++) {
        result.clear();
        for (auto iter = it->docs_->begin(); iter != it->docs_->end(); iter++) {
            v2.push_back(iter->doc_id_);
        }
        set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(result));
        v1 = result;
        v2.clear();
    }
    sort(result.begin(),result.end());
    auto aux = unique(result.begin(), result.end());
    result.erase(aux, result.end());
    return result;
}

static vector<unsigned> union_terms(vector<Term>* &terms){
    vector<unsigned> result;
    vector<unsigned> v1;
    vector<unsigned> v2;
    result.clear();
    v1.clear();
    v2.clear();
    if (terms->size() == 0) {
        cout << "Empty result. Try other query." << endl;
        return result;
    }
    Term term = terms->back();
    for (auto it = term.docs_->begin(); it != term.docs_->end(); it++) {
        v1.push_back(it->doc_id_);
    }
    terms->pop_back();
    
    for (auto it = terms->begin(); it!=terms->end(); it++) {
        result.clear();
        for (auto iter = it->docs_->begin(); iter != it->docs_->end(); iter++) {
            v2.push_back(iter->doc_id_);
        }
        set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(result));
        v1 = result;
        v2.clear();
    }
    sort(result.begin(),result.end());
    auto aux = unique(result.begin(), result.end());
    result.erase(aux, result.end());
    return result;
}


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
    
    string word, op;
    int n,m;
    cin>> n >> op;
    for (int i = 0; i<n; i++) {
        cin >> m;
        vector<Term> *terms = new vector<Term>(m);
        terms->clear();
        /*variable for time measure*/
        float execution_time = 0;
        struct timeval t_start;
        struct timeval t_end;
        
        gettimeofday(&t_start, NULL);
        
        for (int j = 0; j < m; j++) {
            cin >> word;
            int term_id = vocabulary->get_term_id(word);
            long seek = vocabulary->get_seek(term_id);
            Term term = index->read(seek);
            if (term.term_id_ != -1) {
                terms->push_back(term);
            }
        }
        
        vector<unsigned> result;
        if (!op.compare("and")){
            result = intersection_terms(terms);
        } else if (!op.compare("or")){
            result = union_terms(terms);
        }
       for (int n : result) {
            cout << n << ' ';
        }
        cout << endl;
        gettimeofday(&t_end, NULL);
        execution_time = (t_end.tv_sec-t_start.tv_sec)*1000000;
        execution_time = (execution_time+(t_end.tv_usec-t_start.tv_usec))/1000000;
        printf("Execution time %.3f seconds\n", execution_time);
        delete terms;
    }
    delete index;
    return EXIT_SUCCESS;
}