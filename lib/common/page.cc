#include "page.h"

Page::Page(string url, string httpResponse){
    this->url = url;
    parseHTML(HTML::single_blank(HTML::decode_entities(httpResponse)));
}

void Page::parseHTML(string html) {

    HTML::ParserDom parser;
    tree<HTML::Node> dom = parser.parseTree(html);
    tree<HTML::Node>::iterator it = dom.begin();
    this->text += this->url;
    for (; it != dom.end(); ++it) {
        if (it.node != 0 && dom.parent(it) != NULL){
            string parent_tag = dom.parent(it)->tagName();
            //Jump javascript code
            boost::to_lower(parent_tag);
            if (parent_tag == "script" || parent_tag == "noscript" ){
                it.skip_children();
                continue;
            }
        }
        //Parse plain text of the page
        if ((!it->isTag()) && (!it->isComment()) ) {
            this->text += " ";
            this->text += it->text();
        }
        else { //Parse metadata
            string tagName = it->tagName();
            boost::to_lower(tagName);
            if(tagName == "title"){
                it++;
                if(it == dom.end()) return;
                this->title = it->text();
            }
            else if(tagName == "meta"){
                it->parseAttributes();
                std::pair<bool, std::string> attrib = it->attribute("name");
                if(attrib.first == true){
                    boost::to_lower(attrib.second);
                    if(attrib.second == "description")
                        this->description = it->attribute("content").second;
                    if(attrib.second == "keywords")
                        this->keywords = it->attribute("content").second;
                }
                attrib = it->attribute("http-equiv");
                boost::to_lower(attrib.second);
                if(attrib.first == true && attrib.second == "content-type"){
                    this->contentType = it->attribute("content").second;
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
                    this->links[HTML::convert_link(attrib.second, this->url)] = anchor_text;
                    this->text+= " ";
                    this->text += anchor_text;
                }
            }
        }
    }
}