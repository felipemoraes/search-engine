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
#include <sys/time.h>
#include <sys/resource.h>

using namespace std;

unordered_set<string> load_stopwords(string stopwords_directory);
void remove_accents(string &str);

void remove_s(string &s);

bool is_digits(const string &s);

void clean_token(string &s);


class Timer {
    /*variable for time measure*/
public:
    
    struct timeval t_start;
    struct timeval t_end;
    void start(){
        gettimeofday(&t_start, NULL);
    }
    float end(){
        float execution_time = 0;
        gettimeofday(&t_end, NULL);
        execution_time = (t_end.tv_sec-t_start.tv_sec)*1000000;
        execution_time = (execution_time+(t_end.tv_usec-t_start.tv_usec))/1000000;
        
        return execution_time;
    }
};
#endif /* defined(__search_engine__util__) */
