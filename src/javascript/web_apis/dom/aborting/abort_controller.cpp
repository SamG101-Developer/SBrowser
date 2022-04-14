#include "abort_controller.hpp"

#include <dom/aborting/abort_signal.hpp>
#include <dom/helpers/signal_aborting.hpp>


dom::aborting::abort_controller::abort_controller() : dom_object()
{
    // Creates a new AbortController object instance.
    // Returns an AbortSignal object instance, which can be used to communicate with, or to abort, a DOM request.
    signal = new abort_signal{};
}


auto dom::aborting::abort_controller::abort(
        const ext::any& reason) const
        -> void
{
    // The abort(reason) method steps are to signal abort on this’s signal with reason if it is given.
    helpers::signal_aborting::signal_abort(signal, reason);
}


auto dom::aborting::abort_controller::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<abort_controller>{isolate}
            .ctor<>()
            .inherit<dom_object>()
            .function("abort", &abort_controller::abort)
            .var("signal", &abort_controller::signal, true)
            .auto_wrap_objects();
}
