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

Page::Page(string url, string httpResponse){
    url_ = url;
    remove_header(httpResponse);
    parse(httpResponse);
}

static string clean_text(GumboNode* node) {
    if (node->type == GUMBO_NODE_TEXT) {
        return std::string(node->v.text.text);
    } else if (node->type == GUMBO_NODE_ELEMENT &&
               node->v.element.tag != GUMBO_TAG_SCRIPT &&
               node->v.element.tag != GUMBO_TAG_STYLE) {
        std::string contents = "";
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            const std::string text = clean_text((GumboNode*)children->data[i]);
            if (i != 0 && !text.empty()) {
                contents.append(" ");
            }
            contents.append(text);
        }
        return contents;
    } else {
        return "";
    }
}

void Page::search_for_links(GumboNode* node) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }
    GumboAttribute* href;
    if (node->v.element.tag == GUMBO_TAG_A &&
         (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
        GumboVector* children = &node->v.element.children;
        std::string anchor = "";
    
        for (unsigned int i = 0; i < children->length; ++i) {
            const std::string text = clean_text((GumboNode*)children->data[i]);
            if (i != 0 && !text.empty()) {
                anchor.append(" ");
            }
            anchor.append(text);
        }
        string uri;
        try {
            uri = HTML::convert_link(href->value, url_);
        } catch (...) {
            cout << "Fail normalize url: " << href->value << endl;
        }
       
        auto it = links_.find(uri);
        if (it != links_.end()) {
            links_[uri].push_back(anchor);
        } else {
            vector<string> a;
            a.push_back(anchor);
            links_[uri] = a;
        }

    }
    
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        search_for_links(static_cast<GumboNode*>(children->data[i]));
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

void Page::parse(const std::string& html) {
    GumboOutput* output = NULL;
    output = gumbo_parse(html.c_str());
    text_ = clean_text(output->root);
    search_for_links(output->root);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}