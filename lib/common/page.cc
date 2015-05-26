//
//  page.cc
//  search_engine
//  Objective: parse html file, clean it and remove headers
//  Created by Felipe Moraes on 3/20/15.
//
//


#include "page.h"
#include "gumbo.h"
#include <iostream>
using namespace std;
using namespace htmlcxx;

#include "Page.h"

Page::Page(string url, string httpResponse){
    url_ = url;
    //		const char* text = httpResponse.c_str();
    //		if( HTML::detect_utf8(text, httpResponse.size()) == false ){
    //			CharsetConverter cc("8859_1", "UTF8");
    //			httpResponse = string(cc.convert(text));
    //		}
    remove_header(httpResponse);
    text_ = "";
    keywords_ = "";
    title_ = "";
    content_type_ = "";
    description_ = "";
    parse(HTML::single_blank(HTML::decode_entities(httpResponse)));
}

void Page::parse(const std::string& html) {
    
    HTML::ParserDom parser;
    tree<HTML::Node> dom = parser.parseTree(html);
    tree<HTML::Node>::iterator it = dom.begin();
    text_ += url_;
    for (; it != dom.end(); ++it) {
        if(it.node != 0 && dom.parent(it) != NULL){
            string parent_tag = dom.parent(it)->tagName();
            //Pular código javascript
            boost::to_lower(parent_tag);
            if(parent_tag == "script" || parent_tag == "noscript" ){
                it.skip_children();
                continue;
            }
        }
        //Parse plain text of the page
        if ((!it->isTag()) && (!it->isComment()) ) {
            text_ += " ";
            text_ += it->text();
        }
        else { //Parse metadata
            string tagName = it->tagName();
            boost::to_lower(tagName);
            if(tagName == "title"){
                it++;
                if(it == dom.end()) return;
                title_ = it->text();
            }
            else if(tagName == "meta"){
                it->parseAttributes();
                std::pair<bool, std::string> attrib = it->attribute("name");
                if(attrib.first == true){
                    boost::to_lower(attrib.second);
                    if(attrib.second == "description")
                        description_ = it->attribute("content").second;
                    if(attrib.second == "keywords")
                        keywords_ = it->attribute("content").second;
                }
                attrib = it->attribute("http-equiv");
                boost::to_lower(attrib.second);
                if(attrib.first == true && attrib.second == "content-type"){
                    content_type_ = it->attribute("content").second;
                }
            }
            else if(tagName == "a"){
                it->parseAttributes();
                std::pair<bool, std::string> attrib = it->attribute("rel");
                boost::to_lower(attrib.second);
                if(attrib.first == true && attrib.second == "nofollow"){
                    
                }else{
                    attrib = it->attribute("href");
                    string anchor_text;
                    int children = it.number_of_children();
                    for(int i=0; i<children; i++){
                        it++;
                        if(it == dom.end()) return;
                        if(!it->isTag()) anchor_text += it->text();
                    }
                    links_[HTML::convert_link(attrib.second, url_)].push_back(anchor_text);
                    text_ += " ";
                    text_ += anchor_text;
                }
            }
        }
    }
}

bool Page::remove_header(string& str) {
    size_t pos = str.find("<!DOC");
    // either we are already at the beginning of the <html> tag or it doesn't
    // exist.
    if (!pos || pos == string::npos) {
        return false;
    }
    str.erase(0, pos - 1);
    return true;
}