#ifndef WRITER_H_
#define WRITER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "../common/page.h"

using namespace std;


class Writer {

public:
    Writer();
    ~Writer();
    void processPage(Page& p);
    
    
private:
    
    //int run_size_;
    map<string, map<string, int> > dictionary_;
    ofstream temporary_file_;
    
    
};


#endif /* WRITER_H_ */