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
    int run_size = 50;
    vector<File<TermOccurrence>* > runs;
    int max_doc = 2;
    int doc_counter = 0;
    Mapper mapper(run_size,runs);
    while(reader->getNextDocument(doc) && doc_counter < max_doc)	{
        Page p(doc.getURL(), doc.getText());
        mapper.process_page(p);
        doc.clear();
        doc_counter++;
        
    }
    mapper.exec();
    Reducer reducer(run_size,runs);
    reducer.merge();
    
    delete reader;
    
    return EXIT_SUCCESS;
}

