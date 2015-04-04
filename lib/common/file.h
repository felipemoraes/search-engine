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
#include "../common/term_occurrence.h"
using namespace std;

class File {
private:
    int size_;
    int read_;
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
        int position;
        fwrite(&(oc.term_id_), sizeof(int), 1, file_);
        fwrite(&(oc.doc_id_), sizeof(int), 1, file_);
        vector<int>::iterator it;
        vector<int>* positions = oc.get_positions();
        fwrite(&(oc.frequency_), sizeof(int), 1, file_);
        for (it = positions->begin(); it!=positions->end(); it++) {
            position = *it;
            fwrite(&position, sizeof(int), 1, file_);
        }
        
        size_++;
        return size_;
    }
    
    int write_block(TermOccurrence *oc, int block_size){
        ensure_file_is_open();
        for (int i = 0; i<block_size; i++) {
            write(oc[i]);
        }
        return size_;
    }
    
    TermOccurrence read(){
        ensure_file_is_open();
        TermOccurrence oc;
        int position;
        fread((int*) &oc.term_id_, sizeof(oc.term_id_), 1, file_);
        fread((int*) &oc.doc_id_, sizeof(oc.doc_id_), 1, file_);
        fread((int*) &oc.frequency_, sizeof(oc.frequency_), 1, file_);
        for (int i = 0; i<oc.frequency_; i++) {
            fread((int*) &position, sizeof(position), 1, file_);
            oc.add_position(position);
        }
        read_++;
        return oc;
    }
    
    TermOccurrence* read_block(int block_size){
        TermOccurrence *oc = (TermOccurrence*)calloc(block_size, sizeof(TermOccurrence));
        ensure_file_is_open();
        for (int i = 0; i< block_size; i++) {
            oc[i] = read();
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
    
    int get_position(){
        ensure_file_is_open();
        int pointer = ftell(file_);
        int position = pointer/sizeof(TermOccurrence);
        if(feof(file_)){
            return position+1;
        }
        return position;
    }
    
    void set_position(int position){
        ensure_file_is_open();
        fseek(file_, position*sizeof(TermOccurrence), SEEK_SET);
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