//
//  run.cc
//  search_engine
//
//  Created by Felipe Moraes on 3/28/15.
//
//

#include "run.h"


Run::Run(string filename){
    string line;
    file_.open(filename);
    //parse Term
}
Run::~Run(){
    file_.close();
}