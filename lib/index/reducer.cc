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
#include <map>
using namespace std;


Reducer::Reducer(int buffer_size, vector<File<TermOccurrence>* > &runs){
    runs_ = runs;
    buffer_size_ = buffer_size;
}

File<TermOccurrence>* Reducer::merge(){
    
    int block_number = 0;
    int buffer_length;
    TermOccurrence* buffer = new TermOccurrence[buffer_size_];
    stringstream file_name;
    priority_queue< TermOccurrence, vector<TermOccurrence>, greater<TermOccurrence> > heap;
    string directory = "/Users/felipemoraes/Developer/search-engine/data/tmp_files";
    file_name << directory << "/mergedfile" << block_number;
    File<TermOccurrence>* merged = new File<TermOccurrence>(file_name.str());
    map<TermOccurrence, int> indices;
    
    vector<File<TermOccurrence>*>::iterator it;
    for(it = runs_.begin(); it != runs_.end(); it++){
        File<TermOccurrence>* run = *it;
        run->reopen();
        
        TermOccurrence term = run->read();
        heap.push(term);
        indices[term] = it - runs_.begin();
        
        cout << "Run "<< it - runs_.begin() << " size: " << run->get_size() << endl;
    }
    
    while( !heap.empty() ){
        
        TermOccurrence top = heap.top();
        buffer[buffer_length] = top;
        heap.pop();
        buffer_length++;
        if(buffer_length >= buffer_size_){
            merged->write_block(buffer, buffer_size_);
            buffer_length = 0;
        }
        
        int index = indices[top];
        indices.erase(top);
        
        if(runs_[index]->has_next()){
            TermOccurrence head = runs_[index]->read();
            heap.push(head);
            indices[head] = index;
        }
    }
    
    for(it = runs_.begin(); it != runs_.end(); it++){
        File<TermOccurrence>* run = *it;
        run->delete_file();
    }
    
    if(buffer_length > 0){
        merged->write_block(buffer, buffer_length);
        buffer_length = 0;
    }
    cout << "Finished merging " << directory << "/mergedfile" << " with " << merged->get_size() << " entries." << endl;
    
    return merged;
    
    

}