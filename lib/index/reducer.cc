//
//  reducer.cc
//  search_engine
//  Objective: reduce file in one and aggregate it in a index file
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
    
    // merge 1024 runs
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
        // call kmerge for 1024 runs
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
    delete runs;
}


File* Reducer::kmerge(vector<File* >* &runs){
    if (runs->size() <= 1) {
        return runs->front();
    }
    
    // sorting runs with external sort with multi ways
    unsigned buffer_length = 0;
    vector<TermOccurrence>* buffer = new vector<TermOccurrence>();
    stringstream file_name;
    // create heap to be used
    priority_queue< TermOccurrence, vector<TermOccurrence>, greater<TermOccurrence> > *heap;
    heap = new priority_queue< TermOccurrence, vector<TermOccurrence>, greater<TermOccurrence> >();
    string directory = directory_ + "tmp_files";
    file_name << directory << "/run" << block_number_;
    File* merged = new File(file_name.str());
    // initiliaze heap with one occurrence per run file
    for(auto it = runs->begin(); it != runs->end(); it++){
        File* run = *it;
        run->reopen();
        TermOccurrence term = run->read();
        term.run_number_ = it - runs->begin();
        heap->push(term);
        cout << "Run "<< run->get_name() << " size: " << run->get_size() << endl;
    }

    while( !heap->empty() ){
        // get top from heap and put it in buffer
        TermOccurrence top = heap->top();
        buffer->push_back(top);
        heap->pop();
        buffer_length++;
        // check if buffer has been ful and write it in file
        if(buffer_length >= buffer_size_){
            merged->write_block(buffer, buffer_size_);
            buffer->clear();
            buffer_length = 0;
        }
        unsigned index = top.run_number_;
        // replace top for one other occurrence from the same run file
        if((*runs)[index]->has_next()){
            TermOccurrence head = (*runs)[index]->read();
            head.run_number_ = index;
            heap->push(head);
        }
    }
    // delete all run files
    for(auto it = runs->begin(); it != runs->end(); it++){
        File* run = *it;
        run->delete_file();
        delete run;
    }
    // write rest of buffer
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

void Reducer::aggregate(IndexFile &index, Term& aggr, TermOccurrence term, unsigned& previous){
 
    if(previous != term.term_id_){
        if (previous != -1) {
            index.write(aggr);
        }
        previous = term.term_id_;
        aggr.term_id_ = previous;
        aggr.frequency_ = 0;
        aggr.docs_->clear();
    }
    // get frequency of term
    aggr.frequency_++;
    Doc doc;
    doc.frequency_ = term.frequency_;
    doc.doc_id_ = term.doc_id_;
    // get positions of term in document
    vector<unsigned> positions = term.get_positions();
    doc.positions_ = new vector<unsigned>(positions);
    // aggregas it in term aggregation
    aggr.docs_->push_back(doc);
    
}


pair<vector<long>*,vector<long>*> Reducer::reduce(unsigned voc_size, unsigned anchor_size){
    cout << " >> Start reducing" << endl;
    // open merged file and start aggregating it per term_id
    merged_->reopen();
    unsigned term_id_index = -1;
    unsigned term_id_anchor = -1;
    Term aggr_term, aggr_anchor;
    IndexFile* index = new IndexFile(directory_ + "index", voc_size);
    IndexFile* anchor_index = new IndexFile(directory_ + "anchor_index", anchor_size);
    aggr_term.docs_ = new vector<Doc>();
    aggr_anchor.docs_ = new vector<Doc>();
    while (merged_->has_next()) {
        TermOccurrence term = merged_->read();
        // if term_id is differente from other file write it in buffer
        if (term.field_ == 0) {
            aggregate(*index, aggr_term, term, term_id_index);
        } else {
            aggregate(*anchor_index, aggr_anchor, term, term_id_anchor);
        }
        
    }
    index->write(aggr_term);
    merged_->delete_file();
    index->close();
    vector<long>* seeks_voc = index->get_seeks();
    vector<long>* seeks_anchor = anchor_index->get_seeks();
    delete aggr_term.docs_;
    delete index;
    delete anchor_index;
    pair<vector<long>*,vector<long>*> p = make_pair(seeks_voc, seeks_anchor);
    return p;
}