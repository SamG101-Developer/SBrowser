#pragma once
#ifndef SBROWSER_HTML_SLOT_ELEMENT_HPP
#define SBROWSER_HTML_SLOT_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace dom::helpers {struct shadows;}
namespace dom::nodes {class node;}
namespace html::elements {class html_slot_element;}


class html::elements::html_slot_element : public html_element
{
public friends:
    friend struct dom::helpers::shadows;
    friend struct dom::helpers::mutation_algorithms;

public constructors:
    html_slot_element();

public js_methods:
    ext::vector<dom::nodes::node*> assigned_nodes(const ext::string_any_map& options = {});
    ext::vector<dom::nodes::element*> assigned_elements(const ext::string_any_map& options = {});
    template <typename ...nodes_t> void assign(nodes_t... nodes);

public js_properties:
    ext::html_property<ext::string> name;

private cpp_properties:
    ext::vector<dom::nodes::node*>* m_manually_assigned_nodes;
    ext::vector<dom::nodes::node*>* m_assigned_nodes;
};


#endif //SBROWSER_HTML_SLOT_ELEMENT_HPP
