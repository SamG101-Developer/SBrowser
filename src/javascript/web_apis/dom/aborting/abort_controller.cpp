#include "abort_controller.hpp"

#include <dom/aborting/abort_signal.hpp>
#include <dom/helpers/signal_aborting.hpp>

dom::aborting::abort_controller::abort_controller()
        : dom_object()
        , signal(new abort_signal{}) {
}


dom::aborting::abort_controller::~abort_controller() {
    delete signal;
}


void
dom::aborting::abort_controller::abort(ext::cany& reason) {

    helpers::signal_aborting::signal_abort(signal, reason);
}


ext::any
dom::aborting::abort_controller::v8(v8::Isolate* isolate) const {

    return v8pp::class_<abort_controller>{isolate}
            .ctor<>()
            .inherit<dom_object>()
            .function("abort", &abort_controller::abort)
            .var("signal", &abort_controller::signal)
            .auto_wrap_objects();
}
