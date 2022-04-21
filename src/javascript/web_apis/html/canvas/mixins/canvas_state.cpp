#include "canvas_state.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>
#include <html/canvas/offscreen_canvas_rendering_context_2d.hpp>
#include <html/helpers/canvas_internals.hpp>


template <typename T>
auto html::canvas::mixins::canvas_state<T>::save()
        -> void
{
    // push a copy of the drawing state to the stack
    m_drawing_states.push(new internal::drawing_state{*m_current_state});
}


template <typename T>
auto html::canvas::mixins::canvas_state<T>::restore()
        -> void
{
    // return if there are no states in the stack
    if (m_drawing_states.empty())
        return;

    // set the current state to the top of the drawing states, and pop it from the stack
    m_current_state = m_drawing_states.top();
    m_drawing_states.pop();
}


template <typename T>
auto html::canvas::mixins::canvas_state<T>::reset()
        -> void
{
    // reset the canvas element associated with the context that inherits this mixin
    helpers::canvas_internals::reset_canvas_context_to_default_state(static_cast<T*>(this)->canvas);
}


template <typename T>
auto html::canvas::mixins::canvas_state<T>::is_context_lost()
        -> bool
{
    // return if this context has been lost ie if the flag representing this has been set
    return m_context_lost;
}


template <typename T>
auto html::canvas::mixins::canvas_state<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_state<T>>{isolate}
            .template inherit<dom_object>()
            .template function("save", &canvas_state<T>::save)
            .template function("restore", &canvas_state<T>::restore)
            .template function("reset", &canvas_state<T>::reset)
            .template function("isContextLot", &canvas_state<T>::is_context_lost)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_state<html::canvas::canvas_rendering_context_2d>;
template class html::canvas::mixins::canvas_state<html::canvas::offscreen_canvas_rendering_context_2d>;
