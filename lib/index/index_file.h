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
    int doc_id_;
    int frequency_;
    vector<int>* positions_;
};

struct Term {
    int term_id_;
    int frequency_;
    vector<Doc>* docs_;
};


class IndexFile {
    
private:
    int size_;
    int read_;
    string name_;
    FILE* file_;
    
public:
    class FileClosedException {};
    
    IndexFile();
    
    IndexFile(string file_name);
    
    void open(string file_name);
    
    int write(Term oc);
    
    int write_block(Term *oc, int block_size);
    
    Term read();
    
    Term* read_block(int block_size);
    
    void rewind();
    
    void ensure_file_is_open();
    
    bool has_next();
    
    int get_position();
    
    void set_position(int position);
    
    int get_size();
    
    string get_name();
    
    void reopen();
    
    void close();
    
    
    void delete_file();
    
    
    
};


#endif /* defined(__search_engine__index_file__) */
