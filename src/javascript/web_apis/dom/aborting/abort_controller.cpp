#include "abort_controller.hpp"

#include <dom/aborting/abort_signal.hpp>
#include <dom/helpers/signal_aborting.hpp>


dom::aborting::abort_controller::abort_controller()
{
    // set the properties
    signal = new abort_signal{};
}


dom::aborting::abort_controller::~abort_controller()
{
    // delete the signal pointer
    delete signal;
}


auto dom::aborting::abort_controller::abort(
        const ext::any& reason) const
        -> void
{
    // abort the signal with the helper method
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
