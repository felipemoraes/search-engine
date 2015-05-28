//
//  graph.cc
//  search_engine
//
//  Created by Felipe Moraes on 5/12/15.
//
//

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

#include "graph.h"

Graph::Graph(){
    inlinks_ = new unordered_map<int,vector<int> >();
    nodes_ = new unordered_set<int>();
    no_outlinks_ = new unordered_set<int>();
    outlinks_counts_ = new unordered_map<int,int>();
    size_ = 0;
}

Graph::Graph(int size){
    inlinks_ = new unordered_map<int,vector<int> >();
    nodes_ = new unordered_set<int>();
    no_outlinks_ = new unordered_set<int>();
    outlinks_counts_ = new unordered_map<int,int>();
    size_ = size;
}




Graph::~Graph() {
    delete inlinks_;
    delete nodes_;
    delete no_outlinks_;
    delete outlinks_counts_;
}

void Graph::insert(int left, int right){
    nodes_->insert(left);
    nodes_->insert(right);
    if (left == right) {
        return;
    }
    if (inlinks_->find(right) == inlinks_->end()) {
        vector<int> tmp;
        tmp.clear();
        tmp.push_back(left);
        inlinks_->insert(make_pair(right, tmp));
    } else {
        (*inlinks_)[right].push_back(left);
    }
}

unordered_map<int, vector<int> >* Graph::get_inlinks(){
    return inlinks_;
}

    
    
vector<int> Graph::get_inlinks(int p){
    vector<int> result;
    auto it = inlinks_->find(p);
    if (it != inlinks_->end()) {
        result = it->second;
    } else {
        result.clear();
    }
    return result;
}
    
void Graph::compute_out_links(){
    unordered_map<int, vector<int> >* outlinks;
    outlinks = new unordered_map<int,vector<int> >();
    for (auto it = nodes_->begin(); it != nodes_->end(); ++it) {
        vector<int> tmp;
        tmp.clear();
        outlinks->insert(make_pair(*it,tmp));
    }

    for (auto node=nodes_->begin();node!=nodes_->end();++node) {
        for (auto it = (*inlinks_)[*node].begin(); it != (*inlinks_)[*node].end(); ++it) {
            (*outlinks)[*it].push_back(*node);
        }
    }
        
    for (auto node=nodes_->begin();node!=nodes_->end();++node) {
        auto it = outlinks->find(*node);
        if (it == outlinks->end()) {
            no_outlinks_->insert(*node);
        } else {
            outlinks_counts_->insert(make_pair(*node, it->second.size()));
        }
    }
    delete outlinks;
}
    
unordered_set<int>* Graph::get_no_outlinks(){
    return no_outlinks_;
}

int Graph::get_outlink_count(int p){
    auto it = outlinks_counts_->find(p);
    if (it != outlinks_counts_->end()) {
        return it->second;
    }
    return 0;
}
    
int Graph::get_size(){
    return nodes_->size();
}
    
    
vector<float>* Graph::pagerank(int iterations){
    float d = 0.7;
    int i = 0;
    vector<float>* opagerank = new vector<float>();
    vector<float>* npagerank = new vector<float>();
    opagerank->resize(size_);
    npagerank->resize(size_);
    float current_gama = INT32_MAX;
    
    for (int i = 0; i< size_; ++i) {
        (*opagerank)[i] = 1.0;
    }
    
    while ( i++ < iterations && current_gama > 0.00001) {
        current_gama = 0;
        
        for (auto node = 0; node < size_ ;++node) {
            
            float rank = 0;
            
            for (auto ip : get_inlinks(node)) {
                rank += (*opagerank)[ip]/get_outlink_count(ip);
            }
            rank = (1-d) + d*rank;
            
            (*npagerank)[node] = rank;
            
            
        }
        for (int i = 0; i < size_ ; ++i) {
            float diff = abs((*npagerank)[i] - (*opagerank)[i]);
            current_gama = diff > current_gama ? diff : current_gama;
           
        }
        cout << current_gama << endl;

        for (int i = 0; i < size_ ; ++i) {
            (*opagerank)[i] =  (*npagerank)[i];
            
        }
        
    }
    delete npagerank;
    cout << "n iterations " << i << endl;
    return opagerank;
}
