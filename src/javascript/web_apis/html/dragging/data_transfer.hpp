#pragma once
#ifndef SBROWSER_DATA_TRANSFER_HPP
#define SBROWSER_DATA_TRANSFER_HPP

#include <dom_object.hpp>

namespace html::dragging {class data_transfer;}

#include <ext/vector.hpp>

namespace dom::nodes {class element;}
namespace html::dragging {class data_transfer_item;}
namespace html::internal {struct drag_data_store;}


class html::dragging::data_transfer : public virtual dom_object
{
public constructors:
    data_transfer();
    ~data_transfer() override;

public js_methods:
    auto set_drag_image(dom::nodes::element* image, long x, long y) -> void;

public js_properties:
    ext::html_property<ext::string, _F> drop_effect;
    ext::html_property<ext::string, _F> effect_allowed;
    ext::html_property<ext::vector<data_transfer_item*>, _F> items;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private cpp_properties:
    internal::drag_data_store* m_drag_data_store;
    ext::string m_current_drag_operation;
};


#endif //SBROWSER_DATA_TRANSFER_HPP
