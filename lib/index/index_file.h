//
//  index_file.h
//  search_engine
//
//  Created by Felipe Moraes on 4/3/15.
//
//

#ifndef __search_engine__index_file__
#define __search_engine__index_file__

#include <stdio.h>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct Doc {
    unsigned doc_id_;
    unsigned frequency_;
    vector<unsigned>* positions_;
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
    vector<long>* seeks_;
    
public:
    class FileClosedException {};
    
    IndexFile();
    
    IndexFile(const string &file_name);
    
    IndexFile(const string &file_name, const int &size);
    
    void open(const string &file_name, const int &size);
    
    void open(const string &file_name);
    
    int write(const Term &oc);
    
    //FIXME
    int write_block(const Term* &oc, const int &block_size);
    
    void write_size(const int &size);
    
    Term read();
    
    Term read(long seek);
    
    //FIXME
    Term* read_block(const unsigned &block_size);
    
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
    
    vector<long>* get_seeks();
    
    void delete_file();
    
    
    
};


#endif /* defined(__search_engine__index_file__) */
