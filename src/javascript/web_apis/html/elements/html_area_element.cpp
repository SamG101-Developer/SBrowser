#include "html_area_element.hpp"

#include <html/helpers/hyperlinks.hpp>


html::elements::html_area_element::html_area_element()
        : html_element{}
        , mixins::targetable<html_area_element>{}
        , mixins::html_hyperlink_element_utils{}
{
    // constrain the property values
    shape.constrain_values({"circle", "default", "polygon", "rectangle"});
}


auto html::elements::html_area_element::activation_behaviour(
        dom::events::event* event)
        -> void
{
    // return if there is no href
    if (href->empty())
        return;

    // if the download attribute is set, or the user prefers to download, then download the hyperlink
    if (download /* TODO : or preference to download */)
        helpers::hyperlinks::download_hyperlink(this);

    // otherwise, default to following the hyperlink
    else
        helpers::hyperlinks::follow_hyperlink(this);
}


auto html::elements::html_area_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_area_element>{isolate}
            .ctor<>()
            .inherit<html_area_element>()
            .inherit<mixins::targetable<html_area_element>>()
            .inherit<mixins::html_hyperlink_element_utils>()
            .var("alt", html_area_element::alt)
            .var("coords", html_area_element::coords)
            .var("shape", html_area_element::shape)
            .var("download", html_area_element::download)
            .var("ping", html_area_element::ping)
            .var("referrerPolicy", html_area_element::referrerPolicy)
            .auto_wrap_objects();
}
