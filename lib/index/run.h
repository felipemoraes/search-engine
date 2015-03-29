//
//  run.h
//  search_engine
//
//  Created by Felipe Moraes on 3/28/15.
//
//

#ifndef __search_engine__run__
#define __search_engine__run__

#include <iostream>
#include <fstream>
#include <sstream>
#include "../common/term.h"

using namespace std;
class Run {
    
private:
    Term head_;
    ifstream file_;
    
public:
    Run(string filename);
    ~Run();
};


#endif /* defined(__search_engine__run__) */
