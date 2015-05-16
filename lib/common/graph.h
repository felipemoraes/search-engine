//
//  graph.h
//  search_engine
//
//  Created by Felipe Moraes on 5/12/15.
//
//

#ifndef __search_engine__graph__
#define __search_engine__graph__

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

using namespace std;

class Graph {
private:
    int size_;
    unordered_map<int, vector<int> >* inlinks_;
    unordered_set<int>* nodes_;
    unordered_set<int>* no_outlinks_;
    unordered_map<int, int>* outlinks_counts_;
public:
    Graph();
    ~Graph();
    void insert(int left, int right);
    unordered_map<int, vector<int> >* get_inlinks();
    
    vector<int> get_inlinks(int p);
    
    void compute_out_links();
    
    unordered_set<int>* get_no_outlinks();

    int get_outlink_count(int p);
    
    int get_size();
    
    
    vector<float> pagerank(int iterations);
};

#endif /* defined(__search_engine__graph__) */
