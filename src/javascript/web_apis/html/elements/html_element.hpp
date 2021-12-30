#pragma once
#ifndef SBROWSER_HTML_ELEMENT_HPP
#define SBROWSER_HTML_ELEMENT_HPP

#include <dom/nodes/element.hpp>

namespace html {
    namespace elements {class html_element;}
    namespace other {class element_internals;}
}


class html::elements::html_element
        : public dom::nodes::element {
//        , mixins::html_or_svg_element
//        , mixins::element_content_editable {

public: constructors
    html_element();

public: methods
    void click();
    other::element_internals* attach_internals();

public:
    // html
    ext::dom_property<ext::string> title;
    ext::dom_property<ext::string> lang;
    ext::dom_property<ext::string> dir;
    ext::dom_property<ext::string> translate;

    ext::dom_property<ext::string> accessKeyLabel;
    ext::dom_property<ext::string> accessKey{true};
    ext::dom_property<ext::string> autocapitalize{true};
    ext::dom_property<ext::string> innerText{true};
    ext::dom_property<ext::string> outerText{true};

    ext::dom_property<bool> hidden{true};
    ext::dom_property<bool> draggable{true};
    ext::dom_property<bool> spellcheck{true};

    // cssom-view
    ext::dom_property<element*> offsetParent;
    ext::dom_property<long> offsetTop;
    ext::dom_property<long> offsetLeft;
    ext::dom_property<long> offsetWidth;
    ext::dom_property<long> offsetHeight;

private: accessors
    // html
    ext::string get_inner_text() const;
    ext::string get_outer_text() const;

    void set_inner_text(ext::cstring& val);
    void set_outer_text(ext::cstring& val);

    // cssom-view
    long get_offset_top() const;
    long get_offset_left() const;
    long get_offset_width() const;
    long get_offset_height() const;
};


#endif //SBROWSER_HTML_ELEMENT_HPP
