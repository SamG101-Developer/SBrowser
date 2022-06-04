#include "html_map_element.hpp"


html::elements::html_map_element::html_map_element()
        : html_element()
{
    // set the property's values
    areas = new ext::vector<dom::nodes::element*>{};
}


auto html::elements::html_map_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_map_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .var("name", &html_map_element::name)
            .var("areas", &html_map_element::areas)
            .auto_wrap_objects();
}
