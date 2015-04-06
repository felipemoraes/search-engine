#include <iostream>
#include <cstdlib>
#include "../../lib/riCode/CollectionReader.h"
#include <boost/algorithm/string.hpp>
#include <string>
#include <map>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <htmlcxx/html/Node.h>
#include <htmlcxx/html/ParserDom.h>
#include <htmlcxx/html/utils.h>
#include <htmlcxx/html/Uri.h>
#include <htmlcxx/html/CharsetConverter.h>
#include "../../lib/common/page.h"
#include "../../lib/index/mapper.h"
#include "../../lib/index/reducer.h"


using namespace std;
using namespace RICPNS;
using namespace htmlcxx;





int main(int argc, char** argv) {
    
    
    string inputDirectory("/Users/felipemoraes/Developer/search-engine/data/irCollection");
    string indexFileName("index.txt");
    
    CollectionReader * reader = new CollectionReader(inputDirectory,
                                                     indexFileName);
    Document doc;
    doc.clear();
    int run_size = 100;
    vector<File* >* runs;
    int max_doc = 1000;
    int doc_counter = 0;
    
    Mapper mapper(run_size);
    while(reader->getNextDocument(doc) && doc_counter < max_doc) {
        Page p(doc.getURL(), doc.getText());
        mapper.process_page(p);
        doc.clear();
        doc_counter++;
        
    }
    runs = mapper.exec();
    mapper.dump();
    Reducer reducer(run_size,runs);
    reducer.merge();
    reducer.reduce();
    // TODO:
    // - Unit tests
    // - Compress index
    // - Boolean search
    cout << "Doc counter " << doc_counter << endl;
    delete reader;
    delete runs;
    
    return EXIT_SUCCESS;
}

