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
    buffer = new vector<TermOccurrence>;
    buffer_size_ = 0;
    runs_ = new vector<File*>();
    vocabulary_ = new unordered_map<string,unsigned>();
    vocabulary_anchor_ = new unordered_map<string,unsigned>();
    docs_ = new unordered_map<string,pair<unsigned,unsigned> >();
    links_ = new unordered_map<unsigned,vector<unsigned> >();
    doc_file_.open(directory_ + "documents");
    doc_counter_ = 0;
    load_stopwords(stopwords_directory);
}

Mapper::~Mapper(){
    doc_file_.close();
    delete vocabulary_;
    delete vocabulary_anchor_;
    delete buffer;
}

vector<File* >*  Mapper::get_runs(){
    return runs_;
}

void Mapper::process_frequencies(string text, map<string, vector<unsigned> > &positions){
    unsigned position = 0;
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
        if (positions.find(*token)!=positions.end()) {
            positions[*token].push_back(position);
        }
        else {
            pair<string,vector<unsigned> > p;
            p.first = *token;
            p.second.push_back(position);
            positions.insert(p);
        }
        position++;
    }
}

void Mapper::process_page(Page& p){
    
    map<string, vector<unsigned> > positions;
    // get positions from texts
    process_frequencies(p.get_text(),positions);
    unsigned length = 0;
    unsigned page_id = doc_counter_;
    // for each term write it in buffer
    for (auto it = positions.begin(); it != positions.end(); it++){
        unsigned term_id = add_vocabulary(it->first);
        add_buffer(term_id, page_id, it->second,0);
        // check if buffer needs to be write
        flush();
        length++;
    }
    if (docs_->find(p.get_url())==docs_->end()) {
        pair<unsigned,unsigned> tmp = make_pair(doc_counter_,length);
        (*docs_)[p.get_url()] = tmp;
        ++doc_counter_;
    }
    cout << p.get_url() << endl;
    auto links = p.get_links();
    for (auto link : links){
        int doc_id;
        if (docs_->find(link.first)==docs_->end()) {
            (*docs_)[link.first].first = doc_counter_;
            (*docs_)[link.first].second = 0;
            doc_id = doc_counter_;
            (*links_)[page_id].push_back(doc_id);
            ++doc_counter_;
        }
        for (auto text:link.second) {
            if (text == "") {
                continue;
            }
            positions.clear();
            process_frequencies(text, positions);
            for (auto term : positions) {
              //  unsigned term_id = add_vocabulary_anchor(term.first);
             //   add_buffer(term_id, doc_id, term.second,1);
            }
        }
    }
}
// check if buffer size and write it in run file if necesssary
void Mapper::flush(){
    if(buffer_size_ >= run_size_){
        exec();
    }
}

void Mapper::add_buffer(unsigned term_id, unsigned doc_id, vector<unsigned> positions, unsigned field){
    if (field == 1) {
        return;
    }
    TermOccurrence term(term_id, doc_id, positions,field);
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


void Mapper::dump(vector<long>* &seeks_voc, vector<long>* &seeks_anchor){
    string filename(directory_ + "vocabulary");
    ofstream file;
    file.open(filename);
    for (auto it = vocabulary_->begin(); it!= vocabulary_->end(); it++) {
        file << it->first << "\t"<< it->second  << "\t" << (*seeks_voc)[it->second] << endl;
    }
    file.close();
    
    filename = directory_ + "vocabulary_anchor";
    file.open(filename);
    for (auto it = vocabulary_anchor_->begin(); it!= vocabulary_anchor_->end(); it++) {
        file << it->first << "\t"<< it->second  << "\t" << (*seeks_anchor)[it->second] << endl;
    }
    file.close();
    
    for (auto it=docs_->begin(); it!=docs_->end();++it) {
        doc_file_ << it->first <<  "\t" << it->second.first << "\t" << it->second.second<< endl;
    }
}

int Mapper::add_vocabulary(const string& term){
    if (vocabulary_->find(term) != vocabulary_->end()) {
        return (*vocabulary_)[term];
    }
    (*vocabulary_)[term] = voc_counter_;
    voc_counter_++;
    return (*vocabulary_)[term];
}

int Mapper::add_vocabulary_anchor(const string& term){
    if (vocabulary_anchor_->find(term) != vocabulary_anchor_->end()) {
        return (*vocabulary_anchor_)[term];
    }
    (*vocabulary_anchor_)[term] = voc_anchor_counter_;
    voc_anchor_counter_++;
    return (*vocabulary_anchor_)[term];
}

int Mapper::get_vocabulary_size(){
    return vocabulary_->size();
}

int Mapper::get_vocabulary_anchor_size(){
    return vocabulary_anchor_->size();
}

void Mapper::load_stopwords(string stopwords_directory){
    ifstream file;
    string stopwords_list[] = {"portuguese.txt","spanish.txt","english.txt"};
    for (int i = 0; i < 3; i++) {
        file.open(stopwords_directory+stopwords_list[i]);
        string stopword;
        while (!file.eof()) {
            file >> stopword;
            stopwords_.insert(stopword);
        }
        file.close();
    }
}
void Mapper::remove_accents(string &str) {
    for(unsigned int i=0;i<str.length();i++) {
        str.at(i) = tolower(str.at(i));
        unsigned char c = str.at(i);
        if ((c == 0xc3) && ((i+1)<str.length())) {
            str.erase (i,1);
            c = str.at(i);
            c = tolower(c);
        }
        if ((c >= 0x30 && c <= 0x39) || (str.at(i) >= 0x61 && str.at(i) <= 0x7a)) {
            //à, á, â, ã, ä
        }else if (( c >= 0xa0 && c <= 0xa4)){
            str[i]='a';
            //ç
        }else if (c == 0xa7) {
            str[i]='c';
            //è, é, ê , ë
        } else if ((c >= 0xa8 && c <= 0xab)){
            str[i]='e';
            //ì, í, î, ï
        } else if ((c >= 0xac && c <= 0xaf)){
            str[i]='i';
            //ñ
        } else if (c == 0xb1) {
            str[i]='n';
            //ò, ó, ô, õ, ö
        } else if ((c >= 0xb2 && c <= 0xb6)){
            str[i]='o';
            //ù, ú, û, ü
        } else if ((c >= 0xb9 && c <= 0xbc)){
            str[i]='u';
            //Se nao for espaco
        }
        else if(c!=0x20){
            unsigned int x;
            x=str[i];
            if((x>=4294967265)&&(x<=4294967270)){ str[i]='a';}else
                if((x>=4294967272)&&(x<=4294967275)){ str[i]='e';}else
                    if((x>=4294967276)&&(x<=4294967279)) {str[i]='i';}else
                        if(((x>=4294967282)&&(x<=4294967287))||(x==4294967280)){ str[i]='o';}else
                            if(x==4294967281){ str[i]='n';}else
                                if((x>=4294967289)&&(x<=4294967292)) {str[i]='u';}else
                                    if(x==4294967271){ str[i]='c';}else{str.replace (i,1," ");}
        }
    }
}
