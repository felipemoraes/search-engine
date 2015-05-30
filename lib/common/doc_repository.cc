//
//  doc_repository.cc
//  search_engine
//
//  Created by Felipe Moraes on 5/19/15.
//
//

#include "doc_repository.h"


DocRepository::DocRepository(string name) {
    name_ = name;
    documents_ = new unordered_map<unsigned, DocumentInfo>();
    size_ = 0;
}

DocRepository::~DocRepository() {
    delete documents_;
}

void DocRepository::insert(DocumentInfo doc){
    documents_->insert(make_pair(doc.doc_id_, doc));
    size_++;
}
DocumentInfo DocRepository::find(unsigned doc_id){
    DocumentInfo tmp;
    tmp.doc_id_ = doc_id + 1;
    auto it = documents_->find(doc_id);
    if (it != documents_->end()) {
        return it->second;
    }
    return tmp;
}

void DocRepository::dump(string directory){
    file_.open(directory + name_, std::fstream::out);
    for (auto doc : *documents_) {
        doc.second.title_.erase(std::remove(doc.second.title_.begin(), doc.second.title_.end(), '\t'), doc.second.title_.end());
                doc.second.title_.erase(std::remove(doc.second.title_.begin(), doc.second.title_.end(), '\n'), doc.second.title_.end());
        file_ << doc.second.doc_id_ << "\t" << doc.second.url_ << "\t" << doc.second.length_  << "\t" << doc.second.pagerank_ << "\t" << doc.second.title_ << endl;
    }
    file_.close();
}

void DocRepository::load(string directory){
    file_.open(directory + name_, std::fstream::in);
    string line;
    
    vector<string> result;

    while(getline(file_,line)) {
        stringstream ss(line);
        DocumentInfo doc;
        ss >> doc.doc_id_ >> doc.url_ >> doc.length_ >> doc.pagerank_;
        getline(ss, doc.title_);
        insert(doc);

        
    }
    file_.close();
}

void DocRepository::remove(unsigned doc_id){
    auto it = documents_->find(doc_id);
    if (it != documents_->end()) {
        documents_->erase(it);
        size_--;
    }
    
}

unordered_map<unsigned, DocumentInfo>* DocRepository::get_documents(){
    return documents_;
}

