#pragma once
#ifndef SBROWSER_HTML_TEXT_AREA_ELEMENT_HPP
#define SBROWSER_HTML_TEXT_AREA_ELEMENT_HPP

#include <optional>

#include <html/elements/html_element.hpp>
#include <html/mixins/form_associated.hpp>
#include <html/mixins/labelable.hpp>
#include <html/mixins/validatable.hpp>

#include <QtWidgets/QPlainTextEdit>

namespace html::elements {class html_text_area_element;}


class html::elements::html_text_area_element
        : public html_element
        , public mixins::labelable<html_text_area_element>
        , public mixins::form_associated<html_text_area_element>
        , public mixins::validatable<html_text_area_element>
{
public constructors:
    html_text_area_element();

public js_methods:
    auto select() -> void;
    auto set_selection_range(ulong start, ulong end, const ext::string& direction = "none") -> void;
    auto set_range_text(const ext::string& replacement, ext::optional<ulong> start, ext::optional<ulong> end, const ext::string& selection_mode = "preserve") -> void;

public js_properties:
    ext::html_property<ext::string, _T> autocomplete;
    ext::html_property<ext::string, _T> dir_name;
    ext::html_property<ext::string, _T> placeholder;
    ext::html_property<ext::string, _T> wrap;
    ext::html_property<ext::string, _T> default_value;

    ext::html_property<bool, _T> read_only;
    ext::html_property<bool, _T> required;

    ext::html_property<ulong, _T> cols;
    ext::html_property<ulong, _T> rows;
    ext::html_property<ulong, _T> text_length;

    ext::html_property<long, _T> max_length;
    ext::html_property<long, _T> min_length;

    ext::html_property<ulong, _T> selection_start;
    ext::html_property<ulong, _T> selection_end;
    ext::html_property<ext::string, _T> selection_direction;

public cpp_methods:
    auto qt() const -> QPlainTextEdit* override;
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private cpp_properties:
    ext::string m_api_value;
    ext::string m_relevant_value;
    ext::string m_raw_value;
    bool m_dirty_flag;
    ulong m_selection;
    ulong m_text_entry_cursor_position;

private accessors:
    auto get_selection_start() -> ulong;
    auto get_selection_end() -> ulong;
    auto get_selection_direction() -> ext::string;

    auto set_selection_start(ulong val) -> void;
    auto set_selection_end(ulong val) -> void;
    auto set_selection_direction(const ext::string& val) -> void;
};


#endif //SBROWSER_HTML_TEXT_AREA_ELEMENT_HPP
