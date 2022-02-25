#include "abort_controller.hpp"

#include <dom/aborting/abort_signal.hpp>
#include <dom/helpers/signal_aborting.hpp>


dom::aborting::abort_controller::abort_controller()
        : dom_object()
        , signal(std::make_unique<abort_signal>())
{
}



auto dom::aborting::abort_controller::abort(ext::cany& reason) const -> void
{
    // call the helper method to signal abort the signal with a given reason
    helpers::signal_aborting::signal_abort(signal, reason);
}


auto dom::aborting::abort_controller::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<abort_controller>{isolate}
            .ctor<>()
            .inherit<dom_object>()
            .function("abort", &abort_controller::abort)
            .var("signal", &abort_controller::signal)
            .auto_wrap_objects();
}
