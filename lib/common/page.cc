//
//  page.cc
//  search_engine
//
//  Created by Felipe Moraes on 3/20/15.
//
//


#include "page.h"
#include "gumbo.h"
#include <iostream>
using namespace std;

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
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}