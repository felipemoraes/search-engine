//
//  file.cc
//  search_engine
//  Objective: run files operations such as write, read, open and reopen
//  Created by Felipe Moraes on 4/15/15.
//
//


#include <fstream>
#include <cstdio>
#include <string>
#include <stdlib.h>
#include "../common/term_occurrence.h"
#include "file.h"
using namespace std;


    
File::File(){
}
    
File::File(string file_name){
    open(file_name);
}
    
void File::open(string file_name){
    name_ = file_name;
    file_ = fopen(name_.c_str(), "w+b");
    size_ = 0;
}
    
int File::write(TermOccurrence oc){
    ensure_file_is_open();
    fwrite(&(oc.term_id_), sizeof(int), 1, file_);
    fwrite(&(oc.doc_id_), sizeof(int), 1, file_);
    fwrite(&(oc.field_), sizeof(int), 1, file_);
    fwrite(&(oc.frequency_), sizeof(int), 1, file_);
    size_++;
    return size_;
}
    
    int File::write_block(vector<TermOccurrence>* oc, unsigned block_size){
        ensure_file_is_open();
        vector<TermOccurrence>::iterator it;
        for (it=oc->begin(); it!=oc->end(); it++) {
            write(*it);
        }
        return size_;
    }
    
    TermOccurrence File::read(){
        ensure_file_is_open();
        TermOccurrence oc;
        fread((unsigned*) &oc.term_id_, sizeof(oc.term_id_), 1, file_);
        fread((unsigned*) &oc.doc_id_, sizeof(oc.doc_id_), 1, file_);
        fread((unsigned*) &oc.field_, sizeof(oc.field_), 1, file_);
        fread((unsigned*) &oc.frequency_, sizeof(oc.frequency_), 1, file_);
        read_++;
        return oc;
    }
    
    vector<TermOccurrence>* File::read_block(unsigned block_size){
        vector<TermOccurrence> *oc = new vector<TermOccurrence>();
        ensure_file_is_open();
        for (unsigned i = 0; i< block_size; i++) {
            TermOccurrence term = read();
            if (has_next()) {
                oc->push_back(term);
            }
        }
        return oc;
    }
    
    void File::rewind(){
        ensure_file_is_open();
        fseek(file_, 0, SEEK_SET);
    }
    
    void File::ensure_file_is_open(){
        if( file_ == NULL){
            cout << "File " << name_ << " is not open!" << endl;
            throw FileClosedException();
        }
    }
    
    bool File::has_next(){
        ensure_file_is_open();
        if(read_ >= size_){
            return false;
        } else {
            return true;
        }
    }


int File::get_size(){
    ensure_file_is_open();
    return size_;
}
    
string File::get_name(){
    return name_;
}
    
void File::reopen(){
    close();
    file_ = fopen(name_.c_str(), "a+b");
    fseek(file_, 0, SEEK_END);
    rewind();
    read_ = 0;
}
    
void File::close(){
    if(file_ != NULL){
        fclose(file_);
        file_ = NULL;
    }
}
    
void File::delete_file(){
    close();
    remove(name_.c_str());
}


