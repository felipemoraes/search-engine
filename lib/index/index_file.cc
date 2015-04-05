//
//  index_file.cc
//  search_engine
//
//  Created by Felipe Moraes on 4/3/15.
//
//

#include "index_file.h"


IndexFile::IndexFile(){
    
}

IndexFile::IndexFile(string file_name){
    open(file_name);
}


void IndexFile::open(string file_name){
    name_ = file_name;
    file_ = fopen(name_.c_str(), "w+b");
    size_ = 0;
}

int IndexFile::write(Term oc){
    ensure_file_is_open();
    fwrite(&(oc.term_id_), sizeof(int), 1, file_);
    fwrite(&(oc.frequency_), sizeof(int), 1, file_);
    vector<Doc>::iterator it;
    for (it = oc.docs_->begin() ; it!= oc.docs_->end(); it++) {
        fwrite(&(it->doc_id_), sizeof(int), 1, file_);
        fwrite(&(it->frequency_), sizeof(int), 1, file_);
        vector<int>::iterator iter = it->positions_->begin();
        for (; iter!=it->positions_->end(); iter++) {
            fwrite(&(*iter), sizeof(int), 1, file_);
        }
    }
    size_++;
    return size_;
}

int IndexFile::write_block(Term *oc, int block_size){
    ensure_file_is_open();
    for (int i = 0; i<block_size; i++) {
        write(oc[i]);
    }
    return size_;
}

Term IndexFile::read(){
    ensure_file_is_open();
    Term oc;
    fread((int*) &oc.term_id_, sizeof(oc.term_id_), 1, file_);
    fread((int*) &oc.frequency_, sizeof(oc.frequency_), 1, file_);
    oc.docs_ = new vector<Doc>();
    for (int i=0; i<oc.frequency_; i++) {
        Doc doc;
        fread((int*) &(doc.doc_id_), sizeof(doc.doc_id_), 1, file_);
        fread((int*) &(doc.frequency_), sizeof(doc.frequency_), 1, file_);
        doc.positions_ = new vector<int>();
        int position;
        for (int i = 0; i < doc.frequency_; i++) {
            fread((int*) &(position), sizeof(position), 1, file_);
            doc.positions_->push_back(position);
            
        }
    }
    read_++;
    return oc;
}

Term* IndexFile::read_block(int block_size){
    Term *oc = (Term*)calloc(block_size, sizeof(Term));
    ensure_file_is_open();
    for (int i = 0; i< block_size; i++) {
        oc[i] = read();
    }
    return oc;
}


void IndexFile::ensure_file_is_open(){
    if( file_ == NULL){
        cout << "File " << name_ << " is not open!" << endl;
        throw FileClosedException();
    }
}

bool IndexFile::has_next(){
    ensure_file_is_open();
    if(read_ >= size_){
        return false;
    } else {
        return true;
    }
}

int IndexFile::get_size(){
    ensure_file_is_open();
    return size_;
}

string IndexFile::get_name(){
    return name_;
}

void IndexFile::reopen(){
    close();
    file_ = fopen(name_.c_str(), "r+b");
    read_ = 0;
}

void IndexFile::close(){
    if(file_ != NULL){
        fclose(file_);
        file_ = NULL;
    }
}

void IndexFile::delete_file(){
    close();
    remove(name_.c_str());
}

