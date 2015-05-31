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
#include "../../lib/common/util.h"




using namespace std;


void exec(string folder, string filename, vector<RankingModel*> & models, const unordered_map<string, unordered_set<string> > &relevants, string label){
    ofstream ofile;
    ofstream time_file;
    time_file.open(folder+"results.time", std::ofstream::out | std::ofstream::app);
    ModelCombiner combiner;
    ofile.open(folder+filename);
    int query_id = 0;
    vector<Hit> * hits;
    Timer timer;
    for (auto query: relevants) {
        timer.start();
       
        hits = combiner.linear_combiner(models, query.first);
        time_file << label << "\t" <<  hits->size() << "\t" << timer.end() << "\n";
        int i = 0;
        for (auto hit : *hits) {
            ofile << query_id << " Q0 " << hit.doc_.url_ << " " << i << " " << hit.score_ << " " << label<<  endl;
            i++;
        }
        delete hits;
        query_id++;
    }
    time_file.close();
}



int main(int argc, char** argv){
    
    string input_directory("/Users/felipemoraes/Developer/search-engine/data/");
    //consultas-34.txt
    //informacionais.txt
    //transacionais.txt
    string query_file = "transacionais.txt";
    string folder = "/Users/felipemoraes/Developer/search-engine/data/relevantes-34/";
    //Parse comand line arguments
    for(int i=0; i<argc; i++){
        string param(argv[i]);
        if(param == "--directory" || param == "-d"){
            i++;
            input_directory = string(argv[i]);
        }else if(param == "--query_file" || param == "-q"){
            i++;
            query_file = string(argv[i]);
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
    VectorSpaceModel atm(anchor_index,vocabulary_anchor,doc_repository_anchor);
    atm.name_ = "atm";
    vsm.weight_ = 1;
    tm.weight_ = 0.5;
    pgm.weight_ = 0.5;
    atm.weight_ = 2;
    urlm.weight_ = 0.5;
   
   
    string query;
    ifstream file;
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
    
    vector<RankingModel*> models;
    models.push_back(&vsm);
    models.push_back(&tm);
    models.push_back(&urlm);
    exec(folder,"results.vsm", models, relevants, "VSM");
    models.clear();
    
    models.push_back(&vsm);
    models.push_back(&pgm);
    exec(folder,"results.vsm_pgm", models, relevants, "VSM+PageRank");
    models.clear();

    models.push_back(&vsm);
    models.push_back(&atm);
    exec(folder,"results.vsm_atm", models, relevants, "VSM+AnchorText");
    models.clear();
    
    models.push_back(&vsm);
    models.push_back(&tm);
    models.push_back(&urlm);
    models.push_back(&pgm);
    models.push_back(&atm);
    exec(folder,"results.linear", models, relevants, "Linear");
    
    delete vocabulary_anchor;
    delete doc_repository_anchor;
    delete doc_repository;
    delete anchor_index;
    delete vocabulary;
    delete index;
    return EXIT_SUCCESS;
}