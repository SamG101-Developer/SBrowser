#pragma once
#ifndef SBROWSER_HTML_SLOT_ELEMENT_HPP
#define SBROWSER_HTML_SLOT_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace dom::helpers {struct mutation_algorithms;}
namespace dom::helpers {struct shadows;}
namespace dom::nodes {class node;}
namespace dom::nodes {class element;}
namespace dom::nodes {class text;}
namespace html::elements {class html_slot_element;}


class html::elements::html_slot_element : public html_element
{
public friends:
    friend struct dom::helpers::shadows;
    friend struct dom::helpers::mutation_algorithms;

public constructors:
    html_slot_element();

public js_methods:
    auto assigned_nodes(const ext::string_any_map_t& options = {}) -> ext::vector<dom::nodes::node*>*;
    auto assigned_elements(const ext::string_any_map_t& options = {}) -> ext::vector<dom::nodes::element*>*;
    template <typename ...Args> auto assign(Args... nodes) -> void;

public js_properties:
    ext::html_property<ext::string, _F> name;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private cpp_properties:
    ext::vector<dom::nodes::node*>* m_manually_assigned_nodes;
    ext::vector<dom::nodes::node*>* m_assigned_nodes;
};


#endif //SBROWSER_HTML_SLOT_ELEMENT_HPP
