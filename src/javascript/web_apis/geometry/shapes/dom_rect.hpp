#pragma once
#ifndef SBROWSER_DOM_RECT_HPP
#define SBROWSER_DOM_RECT_HPP

#include <geometry/shapes/dom_rect_readonly.hpp>

namespace geometry::shapes {class dom_rect;}


class geometry::shapes::dom_rect : public geometry::shapes::dom_rect_readonly {
public: constructors
    dom_rect(double x = 0, double y = 0, double width = 0, double height = 0);

public: internal_methods
    ext::any&& v8(v8::Isolate *isolate) const override;

private: accessors
    void set_x(double val);
    void set_y(double val);
    void set_width(double val);
    void set_height(double val);
};


#endif //SBROWSER_DOM_RECT_HPP
