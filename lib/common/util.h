//
//  util.h
//  search_engine
//
//  Created by Felipe Moraes on 5/17/15.
//
//

#ifndef __search_engine__util__
#define __search_engine__util__

#include <unordered_set>
#include <fstream>
#include <algorithm>

using namespace std;

unordered_set<string> load_stopwords(string stopwords_directory);
void remove_accents(string &str);

void remove_s(string &s);

bool is_digits(const string &s);

void clean_token(string &s);
#endif /* defined(__search_engine__util__) */
