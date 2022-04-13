#pragma once
#ifndef SBROWSER_CANVAS_FILTER_HPP
#define SBROWSER_CANVAS_FILTER_HPP

#include <ext/record.hpp>
#include <ext/string.hpp>
#include <dom_object.hpp>

namespace html::canvas::paint {class canvas_filter;}


class html::canvas::paint::canvas_filter : public virtual dom_object
{
public aliases:
    using canvas_filter_input_t = ext::record<ext::string, ext::any>;

public constructors:
    canvas_filter(canvas_filter_input_t filters);
    canvas_filter(ext::vector<canvas_filter_input_t> filters = {});

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_FILTER_HPP
