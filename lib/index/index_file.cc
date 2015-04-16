//
//  index_file.cc
//  search_engine
//  Objective: index file operations such as write, read, open and reopen
//  Created by Felipe Moraes on 4/3/15.
//
//

#include "index_file.h"


IndexFile::IndexFile(){
    
}

IndexFile::IndexFile(string file_name){
    name_ = file_name;
    reopen();
    ensure_file_is_open();
    read_size();
}

IndexFile::IndexFile(string file_name, int size){
    open(file_name, size);
}

void IndexFile::open(string file_name, int size){
    name_ = file_name;
    file_ = fopen(name_.c_str(), "w+b");
    write_size(size);
    size_ = 0;
    seeks_ = new vector<long>();
}

void IndexFile::open(string file_name){
    name_ = file_name;
    file_ = fopen(name_.c_str(), "w+b");
    size_ = 0;
    seeks_ = new vector<long>();
}

int IndexFile::write(Term oc){
    ensure_file_is_open();
    seeks_->push_back(ftell(file_));
    fwrite(&(oc.term_id_), sizeof(int), 1, file_);
    fwrite(&(oc.frequency_), sizeof(int), 1, file_);
    for (auto it = oc.docs_->begin() ; it!= oc.docs_->end(); it++) {
        fwrite(&(it->doc_id_), sizeof(int), 1, file_);
        fwrite(&(it->frequency_), sizeof(int), 1, file_);
        for (auto iter = it->positions_->begin(); iter!=it->positions_->end(); iter++) {
            fwrite(&(*iter), sizeof(int), 1, file_);
        }
        delete it->positions_;
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
    fread((unsigned*) &oc.term_id_, sizeof(oc.term_id_), 1, file_);
    fread((unsigned*) &oc.frequency_, sizeof(oc.frequency_), 1, file_);
    oc.docs_ = new vector<Doc>();
    for (unsigned i=0; i<oc.frequency_; i++) {
        Doc doc;
        fread((unsigned*) &(doc.doc_id_), sizeof(doc.doc_id_), 1, file_);
        fread((unsigned*) &(doc.frequency_), sizeof(doc.frequency_), 1, file_);
        doc.positions_ = new vector<unsigned>();
        int position;
        for (unsigned i = 0; i < doc.frequency_; i++) {
            fread((unsigned*) &(position), sizeof(position), 1, file_);
            doc.positions_->push_back(position);
            
        }
    }
    read_++;
    return oc;
}

Term IndexFile::read(long seek){
    ensure_file_is_open();
    fseek(file_, seek, SEEK_SET);
    Term oc;
    fread((unsigned*) &oc.term_id_, sizeof(oc.term_id_), 1, file_);
    fread((unsigned*) &oc.frequency_, sizeof(oc.frequency_), 1, file_);
    oc.docs_ = new vector<Doc>();
    for (int i=0; i<oc.frequency_; i++) {
        Doc doc;
        fread((unsigned*) &(doc.doc_id_), sizeof(doc.doc_id_), 1, file_);
        fread((unsigned*) &(doc.frequency_), sizeof(doc.frequency_), 1, file_);
        doc.positions_ = new vector<unsigned>();
        int position;
        for (int i = 0; i < doc.frequency_; i++) {
            fread((int*) &(position), sizeof(position), 1, file_);
            doc.positions_->push_back(position);
        }
        oc.docs_->push_back(doc);
    }
    read_++;
    return oc;
}

Term* IndexFile::read_block(unsigned block_size){
    Term *oc = (Term*)calloc(block_size, sizeof(Term));
    ensure_file_is_open();
    for (unsigned i = 0; i< block_size; i++) {
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

void IndexFile::write_size(int size){
    fwrite(&(size), sizeof(int), 1, file_);
}

void IndexFile::read_size(){
    fread((int*) &(size_), sizeof(int), 1, file_);
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

vector<long>* IndexFile::get_seeks(){
    return seeks_;
}

