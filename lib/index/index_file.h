//
//  index_file.h
//  search_engine
//  Objective: index file operations such as write, read, open and reopen
//  Created by Felipe Moraes on 4/3/15.
//
//

#ifndef __search_engine__index_file__
#define __search_engine__index_file__

#include <stdio.h>
#include <cstdio>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

struct Doc {
    unsigned doc_id_;
    unsigned frequency_;
};

struct Term {
    unsigned term_id_;
    unsigned frequency_;
    vector<Doc>* docs_;
};


class IndexFile {
    
private:
    int size_;
    int read_;
    string name_;
    FILE* file_;
    unordered_map<unsigned,long>* seeks_;
    
public:
    class FileClosedException {};
    
    IndexFile();
    ~IndexFile();
    
    IndexFile(string file_name);
    
    IndexFile(string file_name, int size);
    
    void open(string file_name, int size);
    
    void open(string file_name);
    
    int write(Term oc);
    
    int write_block(Term *oc, int block_size);
    
    void write_size(int size);
    
    Term read();
    
    Term read(long seek);
    
    Term* read_block(unsigned block_size);
    
    void read_size();
    
    void rewind();
    
    void ensure_file_is_open();
    
    bool has_next();
    
    int get_position();
    
    void set_position(unsigned position);
    
    int get_size();
    
    string get_name();
    
    void reopen();
    
    void close();
    
    unordered_map<unsigned,long>* get_seeks();
    
    void delete_file();
    
    
    
};


#endif /* defined(__search_engine__index_file__) */
