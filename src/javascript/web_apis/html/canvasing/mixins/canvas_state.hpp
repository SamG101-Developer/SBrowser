#pragma once
#ifndef SBROWSER_CANVAS_STATE_HPP
#define SBROWSER_CANVAS_STATE_HPP

#include <dom_object.hpp>

namespace html::canvasing::mixins {template <typename T> class canvas_state;}


template<typename T>
class html::canvasing::mixins::canvas_state : public virtual dom_object
{
public js_methods:
    auto save() -> void;
    auto restore() -> void;
    auto reset() -> void;
    auto is_context_lost() -> bool;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_STATE_HPP
