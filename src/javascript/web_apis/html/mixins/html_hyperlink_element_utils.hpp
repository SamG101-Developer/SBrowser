#pragma once
#ifndef SBROWSER_HTML_HYPERLINK_ELEMENT_UTILS_HPP
#define SBROWSER_HTML_HYPERLINK_ELEMENT_UTILS_HPP

#include <ext/html_property.hpp>
#include <url/url.hpp>
#include <dom_object.hpp>

namespace html::mixins {class html_hyperlink_element_utils;}


class html::mixins::html_hyperlink_element_utils : public virtual dom_object
{
public constructors:
    html_hyperlink_element_utils();

public js_properties:
    ext::html_property<ext::string, _T> href;
    ext::html_property<ext::string, _T> protocol;
    ext::html_property<ext::string, _T> username;
    ext::html_property<ext::string, _T> password;
    ext::html_property<ext::string, _T> host;
    ext::html_property<ext::string, _T> hostname;
    ext::html_property<ext::string, _T> port;
    ext::html_property<ext::string, _T> pathname;
    ext::html_property<ext::string, _T> search;
    ext::html_property<ext::string, _T> hash;
    ext::html_property<ext::string> origin;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

    auto set_the_url() -> void;
    auto reinitialize_url() -> void;

public cpp_properties:
    url::url m_url;

private accessors:
    auto get_href() -> ext::string;
    auto get_protocol() -> ext::string;
    auto get_username() -> ext::string;
    auto get_password() -> ext::string;
    auto get_host() -> ext::string;
    auto get_hostname() -> ext::string;
    auto get_port() -> ext::string;
    auto get_pathname() -> ext::string;
    auto get_search() -> ext::string;
    auto get_hash() -> ext::string;
    auto get_origin() -> ext::string;

    auto set_href(const ext::string& val) -> void;
    auto set_protocol(const ext::string& val) -> void;
    auto set_username(const ext::string& val) -> void;
    auto set_password(const ext::string& val) -> void;
    auto set_host(const ext::string& val) -> void;
    auto set_hostname(const ext::string& val) -> void;
    auto set_port(const ext::string& val) -> void;
    auto set_pathname(const ext::string& val) -> void;
    auto set_search(const ext::string& val) -> void;
    auto set_hash(const ext::string& val) -> void;
};


#endif //SBROWSER_HTML_HYPERLINK_ELEMENT_UTILS_HPP
