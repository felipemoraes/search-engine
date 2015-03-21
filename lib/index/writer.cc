#include "writer.h"



Writer::Writer(){
    cout << "Writer created\n" << endl;
}

Writer::~Writer(){
    
}

void Writer::processPage(Page& p){
    istringstream ss(p.getText());
    string term;
    while (ss >> term) {
        cout << term << endl;
    }
    
}