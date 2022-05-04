#pragma once
#ifndef SBROWSER_HISTORY_HPP
#define SBROWSER_HISTORY_HPP

#include <dom_object.hpp>

namespace html::other {class history;}

namespace dom::nodes {class document;}


class html::other::history : public dom_object
{
public constructors:
    history();

public js_methods:
    auto go(long delta = 0) -> void;
    auto back() -> void;
    auto forward() -> void;
    auto push_state(const ext::any& data, const ext::string& unused, const ext::string& url = "") -> void;
    auto replace_state(const ext::any& data, const ext::string& unused, const ext::string& url = "") -> void;

public js_properties:
    ext::html_property<ulong, _F> length;
    ext::html_property<ext::string, _F> scroll_restoration;
    ext::html_property<ext::any, _F> state;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private cpp_properties:
    dom::nodes::document* m_document;

private accessors:
    auto get_length() -> ulong;
    auto get_scroll_restoration() -> ext::string;
    auto get_state() -> ext::any;

    auto set_scroll_restoration(const ext::string& val) -> void;
};


#endif //SBROWSER_HISTORY_HPP
