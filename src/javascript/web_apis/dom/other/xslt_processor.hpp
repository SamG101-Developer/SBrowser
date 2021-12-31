#pragma once
#ifndef SBROWSER_XSLT_PROCESSOR_HPP
#define SBROWSER_XSLT_PROCESSOR_HPP

#include <ext/macros/decorators.hpp>
#include <ext/types/property.hpp>

namespace dom {
    namespace other {class xslt_processor;}
    namespace nodes {
        class document;
        class document_fragment;
        class node;
    }
}


class dom::other::xslt_processor {
public: constructors
    xslt_processor();

public: methods
    void import_stylesheet(nodes::node* style);

    nodes::document_fragment* transform_to_fragment(nodes::node* source, nodes::document* output);
    nodes::document* transform_to_document(nodes::node* source);

    void set_parameter(ext::cstring& namespace_uri, ext::cstring& local_name, ext::any value);
    ext::any get_parameter(ext::cstring& namespace_uri, ext::cstring& local_name);
    void remove_parameter(ext::cstring& namespace_uri, ext::cstring& local_name);
    void clear_parameters();
    void reset();
};


#endif //SBROWSER_XSLT_PROCESSOR_HPP
