#pragma once
#ifndef SBROWSER_DRAG_TRANSFER_ITEM_HPP
#define SBROWSER_DRAG_TRANSFER_ITEM_HPP

#include <dom_object.hpp>

namespace html::dragging {class drag_transfer_item;}

#include <functional>
#include <optional>

namespace file::objects {class file;}


class html::dragging::drag_transfer_item : public virtual dom_object
{
public aliases:
    using function_callback_t = std::function<void(const ext::string&)>;

public constructors:
    drag_transfer_item();

public js_properties:
    ext::html_property<ext::string, _F> kind;
    ext::html_property<ext::string, _F> type;

public js_methods:
    new_obj auto get_as_string(const function_callback_t& callback) -> void;
    new_obj auto get_as_file() -> file::objects::file;
};


#endif //SBROWSER_DRAG_TRANSFER_ITEM_HPP
