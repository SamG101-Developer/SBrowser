#pragma once
#ifndef SBROWSER_HEADERS_HPP
#define SBROWSER_HEADERS_HPP

#include <ext/maplike.hpp>

namespace fetch::http {class headers;}

#include <ext/record.hpp>
#include <ext/vector.hpp>


class fetch::http::headers : public virtual ext::maplike<ext::string, ext::string>
{
public aliases:
    using headers_init_t = std::variant<ext::vector<ext::vector<ext::string>>, ext::record<ext::string, ext::string>>;
    using header_t = std::pair<ext::string, ext::string>;

public constructors:
    headers(headers_init_t init = {});

public js_methods:
    auto append(const ext::string& name, ext::string& value) -> void;
    auto delete_(const ext::string& name) -> void;
    auto get(const ext::string& name) -> std::optional<ext::string>;
    auto has(const ext::string& name) -> bool;
    auto set(const ext::string& name, ext::string& value) -> void;

private cpp_properties:
    ext::vector<header_t> m_header_list;
    ext::string m_header_guard;
};


#endif //SBROWSER_HEADERS_HPP
