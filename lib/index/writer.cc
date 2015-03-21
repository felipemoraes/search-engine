#include "writer.h"



Writer::Writer(){
    cout << "Writer created\n" << endl;
}

Writer::~Writer(){
    
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