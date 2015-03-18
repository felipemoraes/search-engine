#include <iostream>
#include <cstdlib>
#include "../../lib/riCode/CollectionReader.h"
#include <boost/algorithm/string.hpp>
#include <string>
#include <map>

#include <boost/algorithm/string.hpp>
#include <htmlcxx/html/Node.h>
#include <htmlcxx/html/ParserDom.h>
#include <htmlcxx/html/utils.h>
#include <htmlcxx/html/Uri.h>
#include <htmlcxx/html/CharsetConverter.h>
#include "../../lib/common/page.h"



using namespace std;
using namespace RICPNS;
using namespace htmlcxx;

int main(int argc, char** argv) {
    
    cout << "Testing CollectionReader class..." << endl;
    
    string inputDirectory("/Users/felipemoraes/Developer/search-engine/data/irCollection");
    string indexFileName("index.txt");
    
    CollectionReader * reader = new CollectionReader(inputDirectory,
                                                     indexFileName);
    Document doc;
    doc.clear();

    int i = 0;
    while(reader->getNextDocument(doc))	{
        HTML::ParserDom parser;
        tree<HTML::Node> dom = parser.parseTree(doc.getText());
        tree<HTML::Node>::iterator it = dom.begin();
        cout << "[" << doc.getURL() << "] [" << doc.getLength() << "]" << endl;
        cout << "BEGINHTML[" << doc.getText() << "]ENDHTML" << endl;
        if(doc.getURL() == "http://www.metodoinvestimentos.com.br/parceiros.asp") {
        	cout << "[" << doc.getURL() << "] [" << doc.getLength() << "]" << endl;
        	cout << "BEGINHTML[" << doc.getText() << "]ENDHTML" << endl;
        }
        if((i%1000) == 0) {
            cerr << "[" << doc.getURL() << "]" << endl;
            cerr << i << " processed" << endl;
        }
        Page p(doc.getURL(), doc.getText());
        doc.clear();
        ++i;
    }
    cerr << "Total [" << i << "]" << endl;
    
    delete reader;
    
    cout << "Test finished." << endl;
    
    return EXIT_SUCCESS;
}

