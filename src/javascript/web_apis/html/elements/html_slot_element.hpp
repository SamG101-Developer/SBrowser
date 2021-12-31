#pragma once
#ifndef SBROWSER_HTML_SLOT_ELEMENT_HPP
#define SBROWSER_HTML_SLOT_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace dom {
    namespace helpers {struct shadows;}
    namespace nodes {class node;}
}
namespace html::elements {class html_slot_element;}


class html::elements::html_slot_element : public html_element {
friends
    friend struct dom::helpers::shadows;

public: constructors
    html_slot_element();

public: methods
    ext::vector<dom::nodes::node*> assigned_nodes(ext::cstring_any_map& options = {});
    ext::vector<dom::nodes::element*> assigned_elements(ext::cstring_any_map& options = {});
    template <typename ...nodes> void assign(nodes... nodes);

public: properties
    ext::dom_property<ext::string> name;

private:
    ext::vector<dom::nodes::node*> m_manually_assigned_nodes;
    ext::vector<dom::nodes::node*> m_assigned_nodes;
};


#endif //SBROWSER_HTML_SLOT_ELEMENT_HPP