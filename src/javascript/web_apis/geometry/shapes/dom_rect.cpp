#include "dom_rect.hpp"


geometry::shapes::dom_rect::dom_rect(
        double x,
        double y,
        double width,
        double height)

        : dom_rect_readonly(x, y, width, height) {

    this->x.setter = [this](auto && PH1) {set_x(std::forward<decltype(PH1)>(PH1));};
    this->y.setter = [this](auto && PH1) {set_y(std::forward<decltype(PH1)>(PH1));};
    this->width.setter = [this](auto && PH1) {set_width(std::forward<decltype(PH1)>(PH1));};
    this->height.setter = [this](auto && PH1) {set_height(std::forward<decltype(PH1)>(PH1));};
}


ext::any
geometry::shapes::dom_rect::v8(v8::Isolate* isolate) const {

    return v8pp::class_<dom_rect>{isolate}
        .template ctor<>()
        .template ctor<double>()
        .template ctor<double, double>()
        .template ctor<double, double, double>()
        .template ctor<double, double, double, double>()
        .template inherit<dom_rect_readonly>()

        .template var("x", &dom_rect::x)
        .template var("y", &dom_rect::y)
        .template var("width", &dom_rect::width)
        .template var("height", &dom_rect::height)

        .auto_wrap_objects();
}
