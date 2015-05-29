//
//  mapper.cc
//  search_engine
//  Objective: process texts and write it in sorted files
//  Created by Felipe Moraes on 3/28/15.
//
//


#include "mapper.h"


Mapper::Mapper(unsigned run_size, string index_directory, string stopwords_directory){
    run_size_ = run_size;
    directory_ = index_directory;
    voc_counter_ = 0;
    voc_anchor_counter_ = 0;
    buffer = new vector<TermOccurrence>;
    buffer_size_ = 0;
    runs_ = new vector<File*>();
    vocabulary_ = new unordered_map<string,pair<unsigned,unsigned>>();
    vocabulary_anchor_ = new unordered_map<string,pair<unsigned,unsigned>>();
    docs_ = new DocRepository("documentsInfo");
    docs_anchor_ = new DocRepository("documentsInfoAnchor");
    links_ = new unordered_map<unsigned,vector<unsigned> >();
    urls_ = new unordered_map<string,unsigned>();
    urls_anchor_ = new unordered_map<string,unsigned>();
    doc_counter_ = 0;
    doc_counter_anchor_ = 0;
    stopwords_ = load_stopwords(stopwords_directory);
    outlinks_file_.open(directory_ + "outlinks", std::fstream::out);
}

Mapper::~Mapper(){
    outlinks_file_.close();
    delete vocabulary_;
    delete vocabulary_anchor_;
    delete buffer;
    delete docs_;
    delete docs_anchor_;
    delete urls_;
    delete urls_anchor_;
    delete pagerank_;
    delete links_;
}

vector<File* >*  Mapper::get_runs(){
    return runs_;
}

void Mapper::process_frequencies(string text, map<string, unsigned > &frequencies){
    // remove accents, transform tolower and tokenize text
    remove_accents(text);
    transform(text.begin(), text.end(), text.begin(),::tolower);
    tokenizer<> tokens(text);
    // aggregate terms by positions
    for(auto token = tokens.begin(); token!=tokens.end();++token){
        // check if term is not a stopword
        if (stopwords_.find(*token) != stopwords_.end()) {
            continue;
        }
        // check if term exists in vocabulary
        if (frequencies.find(*token)!=frequencies.end()) {
            frequencies[*token]++;
        }
        else {
            frequencies[*token] = 1;
        }
    }
}

void Mapper::remove_s(string &s){
    if (s.back() == 's') {
       // s = s.substr(0, s.size()-1);
    }
}


void Mapper::process_page(Page& p){
    
    map<string,unsigned> frequencies;
    // get positions from texts
    process_frequencies(p.get_text(),frequencies);
    unsigned length = 0;
    unsigned doc_id = doc_counter_;
    // for each term write it in buffer
    for (auto it = frequencies.begin(); it != frequencies.end(); it++){
        string term = it->first;
        remove_s(term);
        unsigned term_id = add_vocabulary(term);
        add_buffer(term_id, doc_id, it->second,0);
        // check if buffer needs to be write
        flush();
        length++;
    }
    if (urls_->find(p.get_url())==urls_->end()) {
        urls_->insert(make_pair(p.get_url(), doc_counter_));
        DocumentInfo doc;
        doc.doc_id_ = doc_counter_;
        doc.url_ = p.get_url();
        doc.title_ = p.get_title();
        doc.length_ = length;
        docs_->insert(doc);
        ++doc_counter_;
    }
    
    auto links = p.get_links();
    for (auto link : links){
        if (urls_anchor_->find(link.first)==urls_anchor_->end()) {
            DocumentInfo doc;
            doc.doc_id_ = doc_counter_anchor_;
            doc.url_ = link.first;
            (*urls_anchor_)[link.first] = doc_counter_anchor_;
            doc.length_ = 0;
            ++doc_counter_anchor_;
            docs_anchor_->insert(doc);
            
        } 
        frequencies.clear();
        outlinks_file_ << p.get_url() << "\t" << link.first << "\n";
        process_frequencies(link.second, frequencies);
        for (auto term : frequencies) {
            string aux = term.first;
            remove_s(aux);
            unsigned term_id = add_vocabulary_anchor(aux);
            add_buffer(term_id, (*urls_anchor_)[link.first], term.second,1);
        }
    }
}
// check if buffer size and write it in run file if necesssary
void Mapper::flush(){
    if(buffer_size_ >= run_size_){
        exec();
    }
}

void Mapper::add_buffer(unsigned term_id, unsigned doc_id, unsigned frequency, unsigned field){
    TermOccurrence term(term_id, doc_id, frequency,field);
    buffer->push_back(term);
    buffer_size_++;
}

vector<File* >* Mapper::exec(){
    // open file for write buffer
    string directory = directory_ + "tmp_files";
    // sorting buffer
    cout << ">> Flushing buffer of "<< buffer_size_ <<" occurrences to disk..." << endl;
    sort(buffer->begin(), buffer->begin() + buffer_size_);
    int block_number = runs_->size();
    stringstream file_name;
    file_name << directory << "/run" << block_number;
    cout << ">> Writing ordered run in file " << file_name.str() << endl;
    // writing buffer
    File* run_file = new File(file_name.str());
    run_file->write_block(buffer, buffer_size_);
    buffer->clear();
    // keep File object in a runs vector
    runs_->push_back(run_file);
    run_file->close();
    buffer_size_ = 0;
    return runs_;
}


void Mapper::dump(unordered_map<unsigned,long>* &seeks_voc, unordered_map<unsigned,long>* &seeks_anchor){
    string filename(directory_ + "vocabulary");
    ofstream file;
    file.open(filename);
    for (auto it = vocabulary_->begin(); it!= vocabulary_->end(); it++) {
        file << it->first << "\t"<< it->second.first  << "\t" << it->second.second<< "\t" << (*seeks_voc)[it->second.first] << "\n";
    }
    file.close();
    
    filename = directory_ + "vocabulary_anchor";
    file.open(filename);
    for (auto it = vocabulary_anchor_->begin(); it!= vocabulary_anchor_->end(); it++) {
        auto seek = seeks_anchor->find(it->second.first);
        if (seek != seeks_anchor->end()) {
            file << it->first << "\t"<< it->second.first  << "\t"<< it->second.second << "\t" << (*seeks_anchor)[it->second.first] << "\n";
        }
    }
    file.close();
    
    process_pagerank(docs_->size_);
    
    for (auto it=urls_->begin(); it!=urls_->end();++it) {
        DocumentInfo doc = docs_->find(it->second);
        docs_->remove(it->second);
        doc.pagerank_ = (*pagerank_)[it->second];
        docs_->insert(doc);
    }
   
    for (auto it=urls_anchor_->begin(); it!=urls_anchor_->end();++it) {
        auto find_url = urls_->find(it->first);
        if (find_url != urls_->end()) {
            docs_anchor_->remove(it->second);
            it->second = find_url->second;
            DocumentInfo tmp = docs_->find(it->second);
            DocumentInfo doc;
            doc.doc_id_ = it->second;
            doc.url_ = it->first;
            doc.length_ = 0;
            doc.title_ = tmp.title_;
            doc.pagerank_ = (*pagerank_)[it->second];
            docs_anchor_->insert(doc);
        } else {
            docs_anchor_->remove(it->second);
        }
        
    }
    docs_->dump(directory_);
    docs_anchor_->dump(directory_);
    cout << docs_anchor_->size_ << endl;
    delete seeks_anchor;
    delete seeks_voc;
}



void Mapper::process_pagerank(int size){
    Graph graph(size);
    outlinks_file_.close();
    outlinks_file_.open(directory_ + "outlinks", fstream::in);
    while (!outlinks_file_.eof()) {
        string in, out, line;
        getline(outlinks_file_, line);
        stringstream ss(line);
        ss >> in >> out;
        auto find_in = urls_->find(in);
        auto find_out = urls_->find(out);
        if (find_in != urls_->end() && find_out != urls_->end()) {
            graph.insert(find_in->second,find_out->second);
        }
    }
    pagerank_ = graph.pagerank(100);
}

void Mapper::remove_doc_anchor(string doc_url){
    auto it = urls_anchor_->find(doc_url);
    if (it != urls_anchor_->end()) {
        docs_anchor_->remove(it->second);
        urls_anchor_->erase(it);
    }
}

int Mapper::add_vocabulary(const string& term){
    auto it = vocabulary_->find(term);
    if ( it != vocabulary_->end()) {
        it->second.second++;
        return (*vocabulary_)[term].first;
    }
    (*vocabulary_)[term].first = voc_counter_;
    (*vocabulary_)[term].second = 1;
    voc_counter_++;
    return (*vocabulary_)[term].first;
}

int Mapper::add_vocabulary_anchor(const string& term){
    auto it = vocabulary_anchor_->find(term);
    if (it != vocabulary_anchor_->end()) {
        it->second.second++;
        return (*vocabulary_anchor_)[term].first;
    }
    (*vocabulary_anchor_)[term].first = voc_anchor_counter_;
    (*vocabulary_anchor_)[term].second = 0;
    voc_anchor_counter_++;
    return (*vocabulary_anchor_)[term].first;
}


int Mapper::get_vocabulary_size(){
    return vocabulary_->size();
}

int Mapper::get_vocabulary_anchor_size(){
    return vocabulary_anchor_->size();
}

DocRepository* Mapper::get_docs_anchor(){
    return docs_anchor_;
}

unordered_map<string,unsigned >* Mapper::get_urls_anchor(){
    return urls_anchor_;
}

unordered_map<string,unsigned>* Mapper::get_urls(){
    return urls_;
}
