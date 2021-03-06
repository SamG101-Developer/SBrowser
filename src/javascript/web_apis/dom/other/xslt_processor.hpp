#pragma once
#ifndef SBROWSER_XSLT_PROCESSOR_HPP
#define SBROWSER_XSLT_PROCESSOR_HPP

#include <ext/decorators.hpp>
#include <dom_object.hpp>

namespace dom::other {class xslt_processor;}
namespace dom::nodes {class document;}
namespace dom::nodes {class document_fragment;}
namespace dom::nodes {class node;}


class dom::other::xslt_processor : public virtual dom_object
{
public: constructors
    xslt_processor();

public: js_methods
    auto import_stylesheet(nodes::node* style) -> void;

    auto transform_to_fragment(nodes::node* source, nodes::document* output) -> nodes::document_fragment*;
    auto transform_to_document(nodes::node* source) -> nodes::document*;

    auto set_parameter(const ext::string& namespace_uri, const ext::string& local_name, ext::any value) -> void;
    auto get_parameter(const ext::string& namespace_uri, const ext::string& local_name) -> ext::any;
    auto remove_parameter(const ext::string& namespace_uri, const ext::string& local_name) -> void;
    auto clear_parameters() -> void;
    auto reset() -> void;

public: cpp_methods
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_XSLT_PROCESSOR_HPP
