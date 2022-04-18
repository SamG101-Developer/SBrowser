#pragma once
#ifndef SBROWSER_CANVAS_STATE_HPP
#define SBROWSER_CANVAS_STATE_HPP

#include <stack>
#include <dom_object.hpp>

namespace html::canvas::mixins {template <typename T> class canvas_state;}
namespace html::internal {struct drawing_state;}


template<typename T>
class html::canvas::mixins::canvas_state : public virtual dom_object
{
public js_methods:
    auto save() -> void;
    auto restore() -> void;
    auto reset() -> void;
    auto is_context_lost() -> bool;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private cpp_properties:
    internal::drawing_state* m_current_state;
    std::stack<internal::drawing_state*> m_drawing_states;
    bool m_context_lost = false;
};


#endif //SBROWSER_CANVAS_STATE_HPP
