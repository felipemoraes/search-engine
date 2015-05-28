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
#include <sys/time.h>
#include <sys/resource.h>

using namespace std;
using namespace RICPNS;

#define MAX_DOCS 100
#define RUN_SIZE 5


int main(int argc, char** argv) {
    
    /*variable for time measure*/
    float execution_time = 0;
    struct timeval t_start;
    struct timeval t_end;
    
    gettimeofday(&t_start, NULL);
    
    // Start parameters arguments
    string input_directory("/Users/felipemoraes/Developer/search-engine/data/irCollection");
    string index_fileName("index.txt");
    string index_directory("/Users/felipemoraes/Developer/search-engine/data/");
    string stopwords_directory("/Users/felipemoraes/Developer/search-engine/util/stopwords/");
    int num_docs = MAX_DOCS;
    int doc_counter = 0;
    int run_size = RUN_SIZE;
    
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
    run_size = (run_size * 10000000)/40;
    Mapper mapper(run_size, index_directory,stopwords_directory);
    // Read each document from file and execute map phase
    while(reader->getNextDocument(doc) && doc_counter < num_docs) {
        string text = doc.getText();
        Page p(doc.getURL(), doc.getText());
        mapper.process_page(p);
        doc.clear();
        doc_counter++;
    }
    // Execute one last mapper
    runs = mapper.exec();
    Reducer reducer(run_size,runs, index_directory);
    // Merge run files in one
    reducer.merge();
    // start reducing file aggregating by term_id
    reducer.reduce(mapper);
    cout << "Doc counter " << doc_counter << endl;
    cout << "Finished indexer file at " << index_directory + "index" << endl;
    
    delete reader;
    gettimeofday(&t_end, NULL);
    execution_time = (t_end.tv_sec-t_start.tv_sec)*1000000;
    execution_time = (execution_time+(t_end.tv_usec-t_start.tv_usec))/1000000;
    printf("Execution time %.3f minutes\n", execution_time/60);
    
    return EXIT_SUCCESS;
}

