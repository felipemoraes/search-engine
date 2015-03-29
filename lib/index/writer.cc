#include "writer.h"



Writer::Writer(int run_size){
    run_size_ = run_size;
    length_file_ = 0;
    voc_counter_ = 0;
    cout << "Writer created\n" << endl;
}

Writer::~Writer(){
    temporary_file_.close();
}

void Writer::processPage(Page& p){

    map<int, Term> terms;
    int counter = 0;
    string text = p.getText();
    transform(text.begin(), text.end(), text.begin(),::tolower);
    tokenizer<> tokens(text);
    removeAccents(text);
    for(tokenizer<>::iterator token=tokens.begin(); token!=tokens.end();++token){
        string term = *token;
        if (vocabularyContains(term)) {
            int index = getTermIdFromVocabulary(term);
            if (terms.count(index)) {
                terms.find(index)->second.setFrequency();
                terms.find(index)->second.addPosition(counter);
            } else {
                Term t(index,p.getUrl());
                t.setTerm(term);
                t.setFrequency();
                t.addPosition(counter);
                terms.insert(pair<int,Term>(index,t));
            }
        } else {
            int index = addTermToVocabulary(*token);
            Term t(index,p.getUrl());
            t.setTerm(term);
            t.setFrequency();
            t.addPosition(counter);
            
            terms.insert(pair<int,Term>(index,t));
            
        }
        counter++;
        
        
    }
    map<int, Term>::iterator it;
    for (it = terms.begin(); it != terms.end(); it++){
        buffer.push_back(it->second);
        commit();
    }
    
}
string format(vector<Term>::iterator it){
    string str;
    str = it->getTerm() + " " + it->getDocId() + " " + to_string(it->getFrequency()) + " " + it->getPositions();
    return str;
}

bool comp(Term & L, Term & R) {
    if (L.getTerm().compare(R.getTerm()) < 0) {
        return true;
    } else if (L.getTerm().compare(R.getTerm()) == 0){
        if (L.getDocId().compare(R.getDocId()) < 0) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void Writer::commit(){
    if (buffer.size() == run_size_) {
        openRunFile();
        sort(buffer.begin(),buffer.end(),comp);
        vector<Term>::iterator it;
        for (it = buffer.begin(); it != buffer.end(); it++) {
            write(it);
        }
        closeRunFile();
        buffer.clear();
    }
}

void Writer::write (vector<Term>::iterator it){
    string line = format(it);
    temporary_file_ << line << endl;
    
}

void Writer::closeRunFile(){
    temporary_file_.close();
    runs_counter_++;

    
}

void Writer::openRunFile(){
    temporary_file_.open("/Users/felipemoraes/Developer/search-engine/data/tmp_files/run_" + to_string(runs_counter_));
    
}

int Writer::getRunsCounter(){
    return runs_counter_;
}

bool Writer::vocabularyContains(string term){
    if (vocabulary_.count(term)){
        return true;
    } else{
        return false;
    }
                           
}

int Writer::getTermIdFromVocabulary(string term){
    return vocabulary_[term];
}

int Writer::addTermToVocabulary(string term){
    vocabulary_[term] = voc_counter_;
    return voc_counter_++;
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
