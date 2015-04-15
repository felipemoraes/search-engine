//
//  reducer.cc
//  search_engine
//
//  Created by Felipe Moraes on 3/28/15.
//
//

#include "reducer.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <unordered_map>
using namespace std;


Reducer::Reducer(unsigned buffer_size, vector<File* >* &runs, string index_directory){
    runs_ = runs;
    buffer_size_ = buffer_size;
    block_number_ = runs_->size();
    directory_ = index_directory;
}

Reducer::~Reducer(){
    delete merged_;
    delete runs_;
}

void Reducer::merge(){
    cout << ">> Started merging with " << runs_->size() << " blocks" << endl;
    

    vector<File* >* runs = new vector<File*>();
    vector<File*>::iterator it = runs_->begin();
    unsigned count = 0;
    while (true) {
        for (;it!=runs_->end();it++) {
            if (count < 1024) {
                runs->push_back(*it);
                count++;
            } else {
                break;
            }
        }
        cout << " >> Kmerge with " << runs->size() << " blocks" << endl;
 
        File* run = kmerge(runs);
        if (runs->size() <= 1) {
            merged_ = run;
            break;
        } else {
            runs->clear();
            runs->push_back(run);
            count = 1;
        }

    }
    cout << " >> Finished merge step "<< endl;
    //leak memory here
    delete runs;
}


File* Reducer::kmerge(vector<File* >* &runs){
    if (runs->size() <= 1) {
        return runs->front();
    }
    
    
    unsigned buffer_length = 0;
    vector<TermOccurrence>* buffer = new vector<TermOccurrence>();
    stringstream file_name;
    priority_queue< TermOccurrence, vector<TermOccurrence>, greater<TermOccurrence> > *heap;
    heap = new priority_queue< TermOccurrence, vector<TermOccurrence>, greater<TermOccurrence> >();
    string directory = directory_ + "tmp_files";
    file_name << directory << "/run" << block_number_;
    File* merged = new File(file_name.str());
    
    for(auto it = runs->begin(); it != runs->end(); it++){
        File* run = *it;
        run->reopen();
        TermOccurrence term = run->read();
        term.run_number_ = it - runs->begin();
        heap->push(term);
        cout << "Run "<< run->get_name() << " size: " << run->get_size() << endl;
    }
    while( !heap->empty() ){
        TermOccurrence top = heap->top();
        
        buffer->push_back(top);
        heap->pop();
        buffer_length++;
        if(buffer_length >= buffer_size_){
            merged->write_block(buffer, buffer_size_);
            buffer->clear();
            buffer_length = 0;
        }
        unsigned index = top.run_number_;
        if((*runs)[index]->has_next()){
            TermOccurrence head = (*runs)[index]->read();
            head.run_number_ = index;
            heap->push(head);
        }
    }
    
    for(auto it = runs->begin(); it != runs->end(); it++){
        File* run = *it;
        run->delete_file();
        delete run;
    }
    
    if(buffer_length > 0){
        merged->write_block(buffer, buffer_length);
        buffer_length = 0;
    }
    cout << ">> Finished merging " << directory << "/run" << block_number_ << " with " << merged->get_size() << " entries." << endl;
    merged->close();
    block_number_++;
    delete heap;
    delete buffer;
    return merged;
}

vector<long>* Reducer::reduce(unsigned size){
    cout << "Starting reducing" << endl;
    merged_->reopen();
    int term_id = -1;
    Term aggr_term;
    IndexFile* index = new IndexFile(directory_ + "index", size);
    aggr_term.docs_ = new vector<Doc>();
    while (merged_->has_next()) {
        TermOccurrence term = merged_->read();
        if(term_id != term.term_id_){
            if (term_id != -1) {
                index->write(aggr_term);
            }
            term_id = term.term_id_;
            aggr_term.term_id_ = term_id;
            aggr_term.frequency_ = term.frequency_;
            aggr_term.docs_->clear();
        }
        aggr_term.frequency_ += term.frequency_;
        Doc doc;
        doc.frequency_ = term.frequency_;
        doc.doc_id_ = term.doc_id_;
        vector<unsigned> positions = term.get_positions();
        doc.positions_ = new vector<unsigned>();
        for (auto it = positions.begin(); it != positions.end(); it++) {
            doc.positions_->push_back(*it);
        }
        aggr_term.docs_->push_back(doc);
        
    }
    index->write(aggr_term);
    merged_->delete_file();
    index->close();
    vector<long>* seeks = index->get_seeks();
    delete aggr_term.docs_;
    delete index;
    return seeks;
}