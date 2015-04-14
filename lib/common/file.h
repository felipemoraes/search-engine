//
//  file.h
//  search_engine
//
//  Created by Felipe Moraes on 4/01/15.
//
//

#ifndef __search_engine__file__
#define __search_engine__file__


#include <fstream>
#include <cstdio>
#include <string>
#include <stdlib.h>
#include "../common/term_occurrence.h"
using namespace std;

class File {
private:
    unsigned size_;
    unsigned read_;
    string name_;
    FILE* file_;
    
public:
    class FileClosedException {};
    
    File(){
        
    }
    
    File(string file_name){
        open(file_name);
    }
    
    void open(string file_name){
        name_ = file_name;
        file_ = fopen(name_.c_str(), "w+b");
        size_ = 0;
    }
    
    int write(TermOccurrence oc){
        ensure_file_is_open();
        unsigned position;
        fwrite(&(oc.term_id_), sizeof(int), 1, file_);
        fwrite(&(oc.doc_id_), sizeof(int), 1, file_);
        vector<unsigned>::iterator it;
        vector<unsigned> positions = oc.get_positions();
        fwrite(&(oc.frequency_), sizeof(int), 1, file_);
        
        for (it = positions.begin(); it!=positions.end(); it++) {
            position = *it;
            fwrite(&position, sizeof(int), 1, file_);
        }
        size_++;
        return size_;
    }
    
    int write_block(vector<TermOccurrence>* oc, unsigned block_size){
        ensure_file_is_open();
        vector<TermOccurrence>::iterator it;
        for (it=oc->begin(); it!=oc->end(); it++) {
            write(*it);
        }
        return size_;
    }
    
    TermOccurrence read(){
        ensure_file_is_open();
        TermOccurrence oc;
        int position;
        fread((unsigned*) &oc.term_id_, sizeof(oc.term_id_), 1, file_);
        fread((unsigned*) &oc.doc_id_, sizeof(oc.doc_id_), 1, file_);
        fread((unsigned*) &oc.frequency_, sizeof(oc.frequency_), 1, file_);
        for (unsigned i = 0; i<oc.frequency_; i++) {
            fread((int*) &position, sizeof(position), 1, file_);
            oc.add_position(position);
        }
        read_++;
        return oc;
    }
    
    vector<TermOccurrence>* read_block(unsigned block_size){
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
    
    void rewind(){
        ensure_file_is_open();
        fseek(file_, 0, SEEK_SET);
    }
    
    void ensure_file_is_open(){
        if( file_ == NULL){
            cout << "File " << name_ << " is not open!" << endl;
            throw FileClosedException();
        }
    }
    
    bool has_next(){
        ensure_file_is_open();
        if(read_ >= size_){
            return false;
        } else {
            return true;
        }
    }
    
    int get_size(){
        ensure_file_is_open();
        return size_;
    }
    
    string get_name(){
        return name_;
    }
    
    void reopen(){
        close();
        file_ = fopen(name_.c_str(), "a+b");
        fseek(file_, 0, SEEK_END);
        rewind();
        read_ = 0;
    }
    
    void close(){
        if(file_ != NULL){
            fclose(file_);
            file_ = NULL;
        }
    }
    
    void delete_file(){
        close();
        remove(name_.c_str());
    }
    
    

};
#endif /* FILE_H_ */
