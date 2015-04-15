#include <iostream>
#include <cstdlib>
#include "../../lib/riCode/CollectionReader.h"
#include <boost/algorithm/string.hpp>
#include <string>
#include <map>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "../../lib/common/page.h"
#include "../../lib/index/mapper.h"
#include "../../lib/index/reducer.h"

using namespace std;
using namespace RICPNS;

#define MAX_DOCS 9999999
#define RUN_SIZE 5000000


int main(int argc, char** argv) {
    string input_directory("/Users/felipemoraes/Developer/search-engine/data/irCollection");
    string index_fileName("index.txt");
    string index_directory("/Users/felipemoraes/Developer/search-engine/data/");
    string stopwords_directory("/Users/felipemoraes/Developer/search-engine/util/stopwords/");
    int num_docs = MAX_DOCS;
    int doc_counter = 0;
    int run_size = 50;
    
    
    //Parse comand line arguments
    for(int i=0; i<argc; i++){
        string param(argv[i]);
        if(param == "--directory" || param == "-d"){
            i++;
            input_directory = string(argv[i]);
        }
        else if(param == "--filename" || param == "-f"){
            i++;
            index_fileName = string(argv[i]);
        }
        else if(param == "--run_size" || param == "-r"){
            i++;
            if(sscanf(argv[i], "%d", &run_size) == EOF){
                run_size = RUN_SIZE;
            }
        } else if(param == "--num_docs" || param == "-n"){
            i++;
            if(sscanf(argv[i], "%d", &num_docs) == EOF){
                num_docs = MAX_DOCS;
            }
        } else if(param == "-o" || param == "--output"){
            i++;
            index_directory = string(argv[i]);
        }
        else if(param == "-s" || param == "--stopwords"){
            i++;
            stopwords_directory = string(argv[i]);
        }
    }
    Document doc;
    doc.clear();
    vector<File* >* runs;
    CollectionReader* reader = new CollectionReader(input_directory, index_fileName);
    run_size = (run_size * 10000000)/24;
    Mapper mapper(run_size, index_directory,stopwords_directory);
    while(reader->getNextDocument(doc) && doc_counter < num_docs) {
        string text = doc.getText();
        Page p(doc.getURL(), doc.getText());
        mapper.process_page(p);
        doc.clear();
        doc_counter++;
    }
    runs = mapper.exec();
    Reducer reducer(run_size,runs, index_directory);
    reducer.merge();
    int size = mapper.get_vocabulary_size();
    vector<long>* seeks = reducer.reduce(size);
    mapper.dump(seeks);
    cout << "Doc counter " << doc_counter << endl;
    delete reader;
    delete seeks;
    return EXIT_SUCCESS;
}

