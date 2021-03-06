//
//  main.cc
//  search_engine
//
//  Created by Felipe Moraes on 4/10/15.
//
//


#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include "../../lib/index/index_file.h"
#include "../../lib/common/vocabulary.h"
#include "../../lib/search/ranking_model.h"
#include "../../lib/search/vector_space_model.h"
#include "../../lib/search/pagerank_model.h"
#include "../../lib/search/title_model.h"
#include "../../lib/search/url_model.h"
#include "../../lib/common/doc_repository.h"
#include "../../lib/common/hit.h"
#include "../../lib/search/model_combiner.h"
#include "server_ws.hpp"
#include "client_ws.hpp"

//Added for the json-example

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using std::cout;
using std::vector;
using std::string;
using std::stringstream;

using namespace SimpleWeb;
//Added for the json-example:

using namespace boost::property_tree;


int main(int argc, char** argv){
    
    string input_directory("/Users/felipemoraes/Developer/search-engine/data/");
    
    //Parse comand line arguments
    for(int i=0; i<argc; i++){
        string param(argv[i]);
        if(param == "--directory" || param == "-d"){
            i++;
            input_directory = string(argv[i]);
        }
    }
    
    IndexFile* anchor_index = new IndexFile( input_directory + "anchor_index");
    IndexFile* index = new IndexFile( input_directory + "index");
    cout << "Loading vocabulary and documentInfo" << endl;
    Vocabulary* vocabulary = new Vocabulary(input_directory + "vocabulary");
    DocRepository* doc_repository = new DocRepository("documentsInfo");
    Vocabulary* vocabulary_anchor = new Vocabulary(input_directory + "vocabulary_anchor");
    DocRepository* doc_repository_anchor = new DocRepository("documentsInfoAnchor");
    doc_repository->load(input_directory);
    doc_repository_anchor->load(input_directory);
    cout << "Finished loading vocabulary and documentInfo" << endl;
    PageRankModel pgm(index,vocabulary,doc_repository);
    VectorSpaceModel vsm(index,vocabulary,doc_repository);
    vsm.weight_ = 30;
    VectorSpaceModel atm(anchor_index,vocabulary_anchor,doc_repository_anchor);
    atm.name_ = "atm";
    TitleModel tm(index,vocabulary,doc_repository);
    URLModel urlm(index,vocabulary,doc_repository);
    vsm.weight_ = 3;
    tm.weight_ = 0;
    pgm.weight_ = 0.5;
    atm.weight_ = 5;
    urlm.weight_ = 0;
    //WebSocket (WS)-server at port 8080 using 4 threads
    SocketServer<WS> server(8080, 4);
    
    auto& echo=server.endpoint["^/search/?$"];
    
    //C++14, lambda parameters declared with auto
    //For C++11 use: (shared_ptr<Server<WS>::Connection> connection, shared_ptr<Server<WS>::Message> message)
    echo.onmessage=[&server, &pgm, &vsm, &atm, &tm, &urlm](auto connection, auto message) {
        //To receive message from client as string (data_ss.str())
        stringstream data_ss;
        message->data >> data_ss.rdbuf();
        
        cout << "Server: Message received: \"" << data_ss.str() << "\" from " << (size_t)connection.get() << endl;
        
        ptree pt;
        
        read_json(data_ss, pt);
        
        string query;
        string type_search;

        
        query = pt.get<string>("query");
        type_search = pt.get<string>("type_search");
        
        vector<Hit> * hits = new vector<Hit>();
        if (type_search == "linear") {
            vector<RankingModel*> models;
            models.push_back(&pgm);
            models.push_back(&vsm);
            models.push_back(&atm);
            models.push_back(&tm);
            models.push_back(&urlm);
            ModelCombiner combiner;
            hits = combiner.linear_combiner(models, query);
            
        } else if (type_search == "vsm_pgm") {
            vector<RankingModel*> models;
            models.push_back(&pgm);
            models.push_back(&vsm);
            models.push_back(&tm);
            models.push_back(&urlm);
            ModelCombiner combiner;
            hits = combiner.linear_combiner(models, query);
        } else if (type_search == "vsm_atm"){
            vector<RankingModel*> models;
            models.push_back(&atm);
            models.push_back(&vsm);
            models.push_back(&tm);
            models.push_back(&urlm);
            ModelCombiner combiner;
            hits = combiner.linear_combiner(models, query);
        } else if (type_search == "vsm"){
            vector<RankingModel*> models;
            models.push_back(&vsm);
            models.push_back(&tm);
            models.push_back(&urlm);
            ModelCombiner combiner;
            hits = combiner.linear_combiner(models, query);
        }
        
        pt.clear();
        ptree children;
        
        
        
        if (hits->size()) {
            int i = 0;
            for (auto it = hits->begin(); it != hits->end(); ++it) {
                ptree child;
                child.put("title", it->doc_.title_);
                child.put("url", it->doc_.url_);
                children.push_back(make_pair("", child));
                if (i > 100){
                    break;
                }
                i++;
            }
        }
        pt.add_child("hits", children);
        
        // cout << "Server: Sending message \"" << data_ss.str() <<  "\" to " << (size_t)connection.get() << endl;
        
        //server.send is an asynchronous function
        
        stringstream response_ss;
        write_json(response_ss, pt);
        server.send(connection, response_ss, [](const boost::system::error_code& ec){
            if(ec) {
                cout << "Server: Error sending message. " <<
                "Error: " << ec << ", error message: " << ec.message() << endl;
            }
        });
        
        cout << "Server: Message sent " << endl;
    };
    
    echo.onopen=[](auto connection) {
        cout << "Server: Opened connection " << (size_t)connection.get() << endl;
    };
    
    //See RFC 6455 7.4.1. for status codes
    echo.onclose=[](auto connection, int status, const string& reason) {
        cout << "Server: Closed connection " << (size_t)connection.get() << " with status code " << status << endl;
    };
    
    //See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
    echo.onerror=[](auto connection, const boost::system::error_code& ec) {
        cout << "Server: Error in connection " << (size_t)connection.get() << ". " <<
        "Error: " << ec << ", error message: " << ec.message() << endl;
    };
    
    
    thread server_thread([&server](){
        //Start WS-server
        server.start();
    });
    
    server_thread.join();
    
    delete vocabulary;
    delete index;
    return EXIT_SUCCESS;
}
