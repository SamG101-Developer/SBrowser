#pragma once
#ifndef SBROWSER_PATH_2D_HPP
#define SBROWSER_PATH_2D_HPP

#include <ext/map.hpp>
#include <html/canvas/mixins/canvas_path.hpp>

namespace html::canvas::paint {class path_2d;}


class html::canvas::paint::path_2d : public mixins::canvas_path<path_2d>
{
public constructors:
    path_2d(path_2d* path);
    path_2d(const ext::string& path);

public js_methods:
    auto add_path(path_2d* path, const ext::string_any_map& transform = {}) -> void;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_PATH_2D_HPP
