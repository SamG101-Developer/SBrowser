#include "html_menu_element.hpp"


html::elements::html_menu_element::html_menu_element() = default;


auto html::elements::html_menu_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    v8pp::class_<html_menu_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .auto_wrap_objects();
}
