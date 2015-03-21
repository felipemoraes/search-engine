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
        cout << it->first << " " << p.getUrl() << " " << it->second << endl;
    }
    
}

void Writer::write (const string &line){
    if (length_file_ == 0) {
        temporary_file_.open("example.txt");
    } else if (length_file_ <= run_size_) {
        temporary_file_ << line << endl;
    } else {
        temporary_file_.close();
        runs_counter_++;
        length_file_ = 0;
    }
    
}