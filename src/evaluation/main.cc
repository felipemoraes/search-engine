//
//  main.cc
//  search_engine
//
//  Created by Felipe Moraes on 4/10/15.
//
//

#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "../../lib/index/index_file.h"
#include "../../lib/common/vocabulary.h"
#include "../../lib/search/ranking_model.h"
#include "../../lib/search/vector_space_model.h"
#include "../../lib/search/pagerank_model.h"
#include "../../lib/search/title_model.h"
#include "../../lib/search/url_model.h"
#include "../../lib/common/doc_repository.h"
#include "../../lib/common/hit.h"
#include "../../lib/search/model_combiner.h"




using namespace std;



int main(int argc, char** argv){
    
    string input_directory("/Users/felipemoraes/Developer/search-engine/data/");
    
    string query_file = "consultas-34.txt";
    int num_answers = 10;
    string folder = "/Users/felipemoraes/Developer/search-engine/data/relevantes-34/";
    ifstream file;
    //Parse comand line arguments
    for(int i=0; i<argc; i++){
        string param(argv[i]);
        if(param == "--directory" || param == "-d"){
            i++;
            input_directory = string(argv[i]);
        }else if(param == "--query_file" || param == "-q"){
            i++;
            query_file = string(argv[i]);
        }
        else if(param == "--n_answers" || param == "-n"){
            i++;
           num_answers = atoi(argv[i]);
        } else if(param == "--folder" || param == "-f"){
            i++;
            folder = string(argv[i]);
        }
    }
    
    
    IndexFile* anchor_index = new IndexFile( input_directory + "anchor_index");
    IndexFile* index = new IndexFile( input_directory + "index");
    cout << "Loading vocabulary and documentInfo" << endl;
    Vocabulary* vocabulary = new Vocabulary(input_directory + "vocabulary");
    DocRepository* doc_repository = new DocRepository("documentsInfo");
    Vocabulary* vocabulary_anchor = new Vocabulary(input_directory + "vocabulary_anchor");
    DocRepository* doc_repository_anchor = new DocRepository("documentsInfoAnchor");
    doc_repository->load(input_directory);
    doc_repository_anchor->load(input_directory);
    cout << "Finished loading vocabulary and documentInfo" << endl;
    PageRankModel pgm(index,vocabulary,doc_repository);
    VectorSpaceModel vsm(index,vocabulary,doc_repository);
    TitleModel tm(index,vocabulary,doc_repository);
    URLModel urlm(index,vocabulary,doc_repository);
    vsm.weight_ = 1;
    VectorSpaceModel atm(anchor_index,vocabulary_anchor,doc_repository_anchor);
    atm.name_ = "atm";
    atm.weight_ = 1;
   
   
    string query;
    vector<RankingModel*> models;
    models.push_back(&pgm);
    models.push_back(&vsm);
    models.push_back(&atm);
    models.push_back(&tm);
    models.push_back(&urlm);
    ModelCombiner combiner;
    vector<Hit> * hits;
    
    file.open(folder+query_file);
  
    
    unordered_map<string, unordered_set<string> > relevants;
    while(getline(file,query)){
        ifstream fin;
        fin.open(folder+query);
        string url;
        while(getline(fin,url)){
            relevants[query].insert(url);
        }
   
        fin.close();
    }
    file.close();
    ofstream fout;
    fout.open(folder+"results.test");
    int query_id = 0;
    for (auto query: relevants) {
        for (auto relevant : query.second) {
            fout << query_id << " Q0 " << relevant << " 1" << endl;
        }
        for (auto doc : *(doc_repository->get_documents())) {
            if (query.second.find(doc.second.url_) == query.second.end()) {
                fout << query_id << " Q0 " << doc.second.url_ << " 0" << endl;
            }
        }
        query_id++;
    }
    fout.close();

    
    for (auto model : models) {
        ofstream ofile;
        ofile.open(folder + "results." + model->name_);
        file.open(folder+query_file);
        int query_id = 0;
        for (auto query: relevants) {
            cout << model->name_ << " " << query.first << endl;

            hits = model->search(query.first);
            int i = 0;
            for (auto hit : *hits) {
                ofile << query_id << " Q0 " << hit.doc_.url_<< " " << i << " " << hit.score_ << " " << model->name_<<  endl;
                i++;
            }
            query_id++;
            delete hits;
        }
        file.close();
        
    }
    ofstream ofile;
    ofile.open(folder + "results.linear");
    file.open(folder+query_file);
    query_id = 0;
    for (auto query: relevants) {
        hits = combiner.linear_combiner(models, query.first);
        int i = 0;
        for (auto hit : *hits) {
            ofile << query_id << " Q0 " << hit.doc_.url_ << " " << i << " " << hit.score_ << " " << "Linear Combination"<<  endl;
            i++;
        }
        delete hits;
        query_id++;
    }
    
    delete vocabulary_anchor;
    delete doc_repository_anchor;
    delete doc_repository;
    delete anchor_index;
    delete vocabulary;
    delete index;
    return EXIT_SUCCESS;
}