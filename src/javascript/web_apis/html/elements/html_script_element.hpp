#pragma once
#ifndef SBROWSER_HTML_SCRIPT_ELEMENT_HPP
#define SBROWSER_HTML_SCRIPT_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace dom::nodes {class document;}
namespace html::elements {class html_script_element;}
namespace html::helpers {struct script_internals;}


class html::elements::html_script_element : public html_element
{
public friends:
    friend struct helpers::script_internals;

public constructors:
    html_script_element();

public js_methods:
    static auto supports(const ext::string& type) -> bool;

public js_properties:
    ext::html_property<ext::string, _T> src;
    ext::html_property<ext::string, _T> type;
    ext::html_property<ext::string, _T> cross_origin;
    ext::html_property<ext::string, _T> text;
    ext::html_property<ext::string, _T> integrity;
    ext::html_property<ext::string, _T> referrer_policy;

    ext::html_property<bool, _T> no_module;
    ext::html_property<bool, _T> async;
    ext::html_property<bool, _T> defer;

    ext::html_property<ext::string_vector*, _F> blocking;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private js_slots:
    ext::string s_cryptographic_nonce;

private cpp_properties:
    bool m_already_started_flag = false;
    bool m_non_blocking_flag = true;
    bool m_ready_to_be_parser_executed_flag = false;
    bool m_from_external_file_flag = false;
    dom::nodes::document* m_parser_document;
    dom::nodes::document* m_preparation_time_document;
    html_script_element* m_script = nullptr;

private accessors:
    auto get_text() const -> ext::string;
    auto set_text(const ext::string& val) -> void;
};


#endif //SBROWSER_HTML_SCRIPT_ELEMENT_HPP
