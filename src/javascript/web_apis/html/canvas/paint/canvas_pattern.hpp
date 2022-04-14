#pragma once
#ifndef SBROWSER_CANVAS_PATTERN_HPP
#define SBROWSER_CANVAS_PATTERN_HPP

#include <ext/map.hpp>
#include <dom_object.hpp>

namespace html::canvas::paint {class canvas_pattern;}


class html::canvas::paint::canvas_pattern : public virtual dom_object
{
public js_methods:
    auto set_transform(const ext::string_any_map& options = {}) -> void;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_PATTERN_HPP
