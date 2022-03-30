#pragma once
#ifndef SBROWSER_DOM_RECT_READONLY_HPP
#define SBROWSER_DOM_RECT_READONLY_HPP

#include <dom_object.hpp>

namespace geometry::shapes {class dom_rect_readonly;}


class geometry::shapes::dom_rect_readonly : virtual public dom_object {
public: constructors
    dom_rect_readonly(double x = 0, double y = 0, double width = 0, double height = 0);

    ext::dom_property<double> x;
    ext::dom_property<double> y;
    ext::dom_property<double> width;
    ext::dom_property<double> height;

    ext::dom_property<double> top;
    ext::dom_property<double> right;
    ext::dom_property<double> bottom;
    ext::dom_property<double> left;

public: cpp_methods
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private: accessors
    double get_left() const;
    double get_top() const;
    double get_right() const;
    double get_bottom() const;
};


#endif //SBROWSER_DOM_RECT_READONLY_HPP
