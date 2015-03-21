#include "writer.h"



Writer::Writer(int run_size){
    run_size_ = run_size;
    length_file_ = 0;
    cout << "Writer created\n" << endl;
}

Writer::~Writer(){
    temporary_file_.close();
}

void Writer::processPage(Page& p){
    
    map<string, int> terms;
    string text = p.getText();
    transform(text.begin(), text.end(), text.begin(),::tolower);
    tokenizer<> tokens(text);
    for(tokenizer<>::iterator token=tokens.begin(); token!=tokens.end();++token){
        terms[*token]++;
        
    }
    map<string, int>::iterator it;
    for (it = terms.begin(); it != terms.end(); it++){
        string term = it->first;
        string doc_id = p.getUrl();
        string frequence = to_string(it->second);
        write(term+" "+doc_id+" " +frequence);
    }
    
}

void Writer::write (const string &line){
    if (length_file_ == 0) {
        temporary_file_.open("/Users/felipemoraes/Developer/search-engine/data/tmp_files/run_" + to_string(runs_counter_));
        temporary_file_ << line << endl;
        length_file_++;
        runs_counter_++;
    } else if (length_file_ <= run_size_) {
        temporary_file_ << line << endl;
        length_file_++;
    } else {
        temporary_file_.close();
        runs_counter_++;
        length_file_ = 0;
    }
    
}