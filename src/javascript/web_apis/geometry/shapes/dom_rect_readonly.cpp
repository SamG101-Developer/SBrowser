#include "dom_rect_readonly.hpp"


geometry::shapes::dom_rect_readonly::dom_rect_readonly(
        double x,
        double y,
        double width,
        double height)

        : dom_object() {

    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->left.getter = [this] {return get_left();};
    this->top.getter = [this] {return get_top();};
    this->right.getter = [this] {return get_right();};
    this->bottom.getter = [this] {return get_bottom();};
}


double geometry::shapes::dom_rect_readonly::get_left() const {return std::min(x, x + width);}

double geometry::shapes::dom_rect_readonly::get_top() const {return std::min(y, y + height);}

double geometry::shapes::dom_rect_readonly::get_right() const {std::max(x, x + width);}

double geometry::shapes::dom_rect_readonly::get_bottom() const {return std::max(y, y + height);}


ext::any
geometry::shapes::dom_rect_readonly::v8(v8::Isolate* isolate) const {

    return v8pp::class_<dom_rect_readonly>{isolate}
            .template ctor<>()
            .template ctor<double>()
            .template ctor<double, double>()
            .template ctor<double, double, double>()
            .template ctor<double, double, double, double>()
            .template inherit<dom_object>()

            .template var("x", &dom_rect_readonly::x)
            .template var("y", &dom_rect_readonly::y)
            .template var("width", &dom_rect_readonly::width)
            .template var("height", &dom_rect_readonly::height)
            .template var("left", &dom_rect_readonly::left)
            .template var("top", &dom_rect_readonly::top)
            .template var("right", &dom_rect_readonly::right)
            .template var("bottom", &dom_rect_readonly::bottom)

            .auto_wrap_objects();
}
