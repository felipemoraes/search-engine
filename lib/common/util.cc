//
//  util.cpp
//  search_engine
//
//  Created by Felipe Moraes on 5/17/15.
//
//

#include "util.h"


unordered_set<string> load_stopwords(string stopwords_directory){
    unordered_set<string> stopwords;
    ifstream file;
    string stopwords_list[] = {"portuguese.txt","spanish.txt","english.txt"};
    for (int i = 0; i < 3; i++) {
        file.open(stopwords_directory+stopwords_list[i]);
        string stopword;
        while (!file.eof()) {
            file >> stopword;
            stopwords.insert(stopword);
        }
        file.close();
    }
    return stopwords;
}


void remove_accents(string &str) {
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