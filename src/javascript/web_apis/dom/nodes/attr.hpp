#pragma once
#ifndef SBROWSER_ATTR_HPP
#define SBROWSER_ATTR_HPP

#include <dom/nodes/node.hpp>

namespace dom::nodes {class attr;}
namespace dom::nodes {class element;}


class dom::nodes::attr : public node
{
public constructors:
    attr();

public js_properties:
    ext::property<ext::string> namespace_uri;
    ext::property<ext::string> prefix;
    ext::property<ext::string> local_name;
    ext::property<ext::string> name;
    ext::property<ext::string> value;
    ext::property<smart_pointer<element>> owner_element;

public: cpp_methods
    auto qt() const -> QWidget* override {return nullptr;}
    auto v8(v8::Isolate* isolate) const -> ext::any override;

protected cpp_properties:
    ext::property<ext::string> m_qualified_name;

private accessors:
    [[nodiscard]] auto get_node_value() const -> ext::string& override;
    [[nodiscard]] auto get_text_content() const -> ext::string& override;
    [[nodiscard]] auto get_m_qualified_name() const -> ext::string;

    auto set_node_value(const ext::string& val) -> void override;
    auto set_text_content(const ext::string& val) -> void override;
    auto set_value(const ext::string& val) -> void;
};


#endif //SBROWSER_ATTR_HPP
