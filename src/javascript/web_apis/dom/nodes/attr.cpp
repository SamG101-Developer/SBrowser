#include "attr.hpp"

#include <dom/helpers/attributes.hpp>


dom::nodes::attr::attr() : node() {

    node_value.get       = [this] {return get_node_value(); };
    text_content.get     = [this] {return get_text_content(); };
    m_qualified_name.get = [this] {return get_m_qualified_name(); };

    value.set        = [this](auto&& PH1) {set_value(std::forward<decltype(PH1)>(PH1));};
    node_value.set   = [this](auto&& PH1) {set_node_value(std::forward<decltype(PH1)>(PH1));};
    text_content.set = [this](auto&& PH1) {set_text_content(std::forward<decltype(PH1)>(PH1));};
}


ext::string dom::nodes::attr::get_node_value() const {return value;}

ext::string dom::nodes::attr::get_text_content() const {return value;}

ext::string dom::nodes::attr::get_m_qualified_name() const {return ext::vector<ext::string>{prefix, ":", local_name}.join();}

void dom::nodes::attr::set_node_value(ext::cstring& val) {set_value(val);}

void dom::nodes::attr::set_text_content(ext::cstring& val) {set_value(val);}

void dom::nodes::attr::set_value(ext::cstring& val) {helpers::attributes::set_existing_attribute_value(this, val);}
