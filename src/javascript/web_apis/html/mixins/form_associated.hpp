#pragma once
#ifndef SBROWSER_FORM_ASSOCIATED_HPP
#define SBROWSER_FORM_ASSOCIATED_HPP

#include <ext/decorators.hpp>
#include <ext/string.hpp>
#include <dom_object.hpp>

namespace html::elements {class html_element;}
namespace html::elements {class html_form_element;}
namespace html::mixins {template <typename T> class form_associated;}

ext::string_vector form_associated_elements {"button", "output", "select", "textarea", "img"}; // TODO : namespace


template <typename T>
class html::mixins::form_associated : public virtual dom_object
{
public enums:
    enum class validity_flags : int
    {
        VALUE_MISSING    = 0x01,
        TYPE_MISMATCH    = 0x02,
        PATTERN_MISMATCH = 0x04,
        TOO_LONG         = 0x08,
        RANGE_UNDERFLOW  = 0x10,
        RANGE_OVERFLOW   = 0x20,
        STEP_MISMATCH    = 0x40,
        CUSTOM_ERROR     = 0x80
    };

public constructors:
    form_associated();

public js_properties:
    ext::html_property<ext::string, _T> name;
    ext::html_property<ext::string, _F> type;
    ext::html_property<ext::string, _F> value;
    ext::html_property<elements::html_form_element*, _F> form;
    ext::html_property<bool, _T> disabled;

public js_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

protected cpp_methods:
    // TODO : reset_algorithm() etc? ...

protected cpp_properties:
    bool m_parser_inserted_flag = false;
    std::tuple<ext::string, ext::string> m_submission_value;
    std::tuple<ext::string, ext::string> m_state;

    int m_validity_flags;
    ext::string m_validation_message;
    elements::html_element* m_validation_anchor;
};


#endif //SBROWSER_FORM_ASSOCIATED_HPP
