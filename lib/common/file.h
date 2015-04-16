//
//  file.h
//  search_engine
//  Objective: run files operations such as write, read, open and reopen
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
    
    File();
    
    File(string file_name);
    
    void open(string file_name);
    
    int write(TermOccurrence oc);
    
    int write_block(vector<TermOccurrence>* oc, unsigned block_size);
    TermOccurrence read();
    vector<TermOccurrence>* read_block(unsigned block_size);
    void rewind();
    void ensure_file_is_open();
    bool has_next();
    int get_size();
    string get_name();
    void reopen();
    void close();
    void delete_file();
    

};
#endif /* FILE_H_ */
