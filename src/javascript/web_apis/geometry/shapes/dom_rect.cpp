#include "dom_rect.hpp"


geometry::shapes::dom_rect::dom_rect(
        double x,
        double y,
        double width,
        double height)

        : dom_rect_readonly(x, y, width, height) {

    this->x.set = [this](auto && PH1) {set_x(std::forward<decltype(PH1)>(PH1));};
    this->y.set = [this](auto && PH1) {set_y(std::forward<decltype(PH1)>(PH1));};
    this->width.set = [this](auto && PH1) {set_width(std::forward<decltype(PH1)>(PH1));};
    this->height.set = [this](auto && PH1) {set_height(std::forward<decltype(PH1)>(PH1));};
}


ext::any
geometry::shapes::dom_rect::v8(v8::Isolate* isolate) const {

    return v8pp::class_<dom_rect>{isolate}
        .ctor<>()
        .ctor<double>()
        .ctor<double, double>()
        .ctor<double, double, double>()
        .ctor<double, double, double, double>()
        .inherit<dom_rect_readonly>()

        .var("x", &dom_rect::x)
        .var("y", &dom_rect::y)
        .var("width", &dom_rect::width)
        .var("height", &dom_rect::height)

        .auto_wrap_objects();
}
