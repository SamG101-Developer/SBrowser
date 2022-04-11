#pragma once
#ifndef SBROWSER_CANVAS_TRANSFORM_HPP
#define SBROWSER_CANVAS_TRANSFORM_HPP

#include <dom_object.hpp>
#include <ext/map.hpp>

namespace html::canvasing::mixins {template <typename T> class canvas_transform;}
namespace geometry::abstract {class dom_matrix;}


template<typename T>
class html::canvasing::mixins::canvas_transform : public virtual dom_object
{
public js_methods:
    auto scale(double x, double y) -> void;
    auto rotate(double angle) -> void;
    auto translate(double x, double y) -> void;
    auto transform(double a, double b, double c, double d, double e, double f) -> void;

    auto reset_transformation() -> void;
    auto set_transform(const ext::string_any_map& options) -> void;
    new_obj auto get_transform() const -> geometry::abstract::dom_matrix;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_TRANSFORM_HPP
