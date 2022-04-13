#pragma once
#ifndef SBROWSER_CANVAS_USER_INTERFACE_HPP
#define SBROWSER_CANVAS_USER_INTERFACE_HPP

#include <dom_object.hpp>

namespace dom::nodes {class element;}
namespace html::canvas::mixins {template<typename T> class canvas_user_interface;}
namespace html::canvas::paint {class path_2d;}


template<typename T>
class html::canvas::mixins::canvas_user_interface : virtual public dom_object
{
public js_methods:
    auto draw_focus_if_needed(dom::nodes::element* element) -> void;
    auto draw_focus_if_needed(paint::path_2d* path, dom::nodes::element* element) -> void;
    auto scroll_path_into_view() -> void;
    auto scroll_path_into_view(paint::path_2d* path) -> void;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_USER_INTERFACE_HPP
