#pragma once
#ifndef SBROWSER_LOCATION_HPP
#define SBROWSER_LOCATION_HPP

#include <dom_object.hpp>

namespace html::other {class location;}

namespace dom::nodes {class document;}


class html::other::location : public virtual dom_object
{
public constructors:
    location();

public js_methods:
    auto assign(const ext::string& url) -> void;
    auto replace(const ext::string& url) -> void;
    auto reload() -> void;

public js_properties:
    ext::html_property<ext::string, _F> href;
    ext::html_property<ext::string, _F> origin;
    ext::html_property<ext::string, _F> protocol;
    ext::html_property<ext::string, _F> host;
    ext::html_property<ext::string, _F> hostname;
    ext::html_property<ext::string, _F> port;
    ext::html_property<ext::string, _F> pathname;
    ext::html_property<ext::string, _F> search;
    ext::html_property<ext::string, _F> hash;
    ext::html_property<ext::string_vector, _F> ancestor_origins;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

protected cpp_properties:
    dom::nodes::document* m_relevant_document;

private accessors:
    auto get_href() const -> const ext::string&;
    auto get_origin() const -> const ext::string&;
    auto get_protocol() const -> const ext::string&;
    auto get_host() const -> const ext::string&;
    auto get_hostname() const -> const ext::string&;
    auto get_port() const -> const ext::string&;
    auto get_pathname() const -> const ext::string&;
    auto get_search() const -> const ext::string&;
    auto get_hash() const -> const ext::string&;
    auto get_ancestor_origins() const -> ext::string_vector&;

    auto set_href(const ext::string&) -> void;
    auto set_origin(const ext::string&) -> void;
    auto set_protocol(const ext::string&) -> void;
    auto set_host(const ext::string&) -> void;
    auto set_hostname(const ext::string&) -> void;
    auto set_port(const ext::string&) -> void;
    auto set_pathname(const ext::string&) -> void;
    auto set_search(const ext::string&) -> void;
    auto set_hash(const ext::string&) -> void;
};


#endif //SBROWSER_LOCATION_HPP
