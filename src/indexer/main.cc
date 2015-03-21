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
#include "../../lib/index/writer.h"


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
    Writer writer(run_size);
    while(reader->getNextDocument(doc))	{
        Page p(doc.getURL(), doc.getText());
        writer.processPage(p);
        doc.clear();
    }
    
    delete reader;
    
    return EXIT_SUCCESS;
}

