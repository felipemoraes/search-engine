
#ifndef PAGE_H_
#define PAGE_H_

#include <string>
#include <map>

#include <boost/algorithm/string.hpp>
#include <htmlcxx/html/Node.h>
#include <htmlcxx/html/ParserDom.h>
#include <htmlcxx/html/utils.h>
#include <htmlcxx/html/Uri.h>
#include <htmlcxx/html/CharsetConverter.h>

using namespace std;
using namespace htmlcxx;

class Page {
  public:

    Page(string url, string httpResponse);

    string getText();
    string getUrl();
    string getTitle();
    string getKeywords();
    string getDescription();
    string getContentType();
    map<string, string> getLinks();

  private:

    void parseHTML(string html);

    string url;
    string text;
    string title;
    string keywords;
    string description;
    string contentType;
    map<string, string> links;
	
};

inline string Page::getText(){
    return text;
}

inline string Page::getUrl(){
    return url;
}

inline string Page::getTitle(){
    return title;
}

inline string Page::getKeywords(){
    return keywords;
}

inline string Page::getDescription(){
    return description;
}

inline string Page::getContentType(){
    return contentType;
}

inline map<string, string> Page::getLinks(){
    return links;
}

#endif /* PAGE_H_ */