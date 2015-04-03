//
//  writer.cc
//  search_engine
//
//  Created by Felipe Moraes on 3/28/15.
//
//


#include "writer.h"

Writer::Writer(int run_size, vector<File<TermOccurrence>* > &runs){
    run_size_ = run_size;
    voc_counter_ = 0;
    buffer = new TermOccurrence[run_size];
    buffer_size_ = 0;
    cout << "Writer created\n" << endl;
    runs_ = runs;
}

Writer::~Writer(){
}

vector<File<TermOccurrence>* >  Writer::get_runs(){
    return runs_;
}

void Writer::processFrequencies(Page& p, map<string,int> &frequencies){
    
    string text = p.getText();
    removeAccents(text);
    transform(text.begin(), text.end(), text.begin(),::tolower);
    tokenizer<> tokens(text);
    for(tokenizer<>::iterator token=tokens.begin(); token!=tokens.end();++token){
        string term = *token;
        if (frequencies.count(*token)) {
            frequencies[*token]++;
        }
        else {
            frequencies.insert(pair<string,int>(*token,1));
        }
    }
}

void Writer::processPage(Page& p){

    map<string, int> frequencies;
    processFrequencies(p,frequencies);
    map<string, int>::iterator it;
    for (it = frequencies.begin(); it != frequencies.end(); it++){
        int term_id = add_vocabulary(it->first);
        int doc_id = doc_counter_++;
        add_buffer(term_id, doc_id,it->second);
        flush();
    }
    
}

void Writer::flush(){
    if(buffer_size_ >= run_size_){
        commit();
    }
}

void Writer::add_buffer(int term_id, int doc_id, int frequency){
    TermOccurrence term(term_id, doc_id,frequency);
    buffer[buffer_size_] = term;
    buffer_size_++;
}

void Writer::commit(){
    string directory = "/Users/felipemoraes/Developer/search-engine/data/tmp_files";
    if(buffer_size_ <= 0) return;
    cout << ">> Flushing buffer of "<< buffer_size_ <<" occurrences to disk..." << endl;
    sort(buffer,buffer+buffer_size_);
    int block_number = runs_.size();
    stringstream file_name;
    file_name << directory << "/run" << block_number;
    cout << "Writing ordered run in file " << file_name.str() << endl;
    File<TermOccurrence>* run_file = new File<TermOccurrence>(file_name.str());
    run_file->write_block(buffer, buffer_size_);
    runs_.push_back(run_file);
    run_file->close();
    buffer_size_ = 0;
}

int Writer::add_vocabulary(string term){
    if (vocabulary_.count(term)) {
        return vocabulary_[term];
    }
    vocabulary_[term] = voc_counter_;
    voc_counter_++;
    return vocabulary_[term];
}


void Writer::removeAccents(string &str) {
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
