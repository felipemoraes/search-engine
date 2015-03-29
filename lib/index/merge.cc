//
//  merge.cc
//  search_engine
//
//  Created by Felipe Moraes on 3/28/15.
//
//

#include "merge.h"
#include "run.h"
#include <queue>

Merge::Merge(){
    priority_queue<Run, vector<Run>, std::greater<Run> > heap;
}