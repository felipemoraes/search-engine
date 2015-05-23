//
//  hit.cc
//  search_engine
//
//  Created by Felipe Moraes on 5/23/15.
//
//

#include "hit.h"

Hit::Hit(DocumentInfo doc, float score){
    doc_ = doc;
    score_ = score;
}