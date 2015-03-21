#ifndef WRITER_H_
#define WRITER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include<boost/tokenizer.hpp>
#include "../common/page.h"


using namespace std;
using namespace boost;


class Writer {

public:
    Writer(int run_size);
    ~Writer();
    void processPage(Page& p);
    void write (const string &line);
    
private:
    
    int run_size_;
    int runs_counter_;
    int length_file_;
    ofstream temporary_file_;
    
    
};


#endif /* WRITER_H_ */