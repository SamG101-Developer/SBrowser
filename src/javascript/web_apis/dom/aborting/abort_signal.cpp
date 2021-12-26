#include "abort_signal.hpp"

#include <dom/helpers/exceptions.hpp>


dom::aborting::abort_signal::abort_signal()
        : nodes::event_target() {
}


dom::aborting::abort_signal
dom::aborting::abort_signal::abort(
        std::any&& reason) {

    reason = reason.has_value() ? reason : other::dom_exception{"", ABORT_ERR};

    abort_signal signal{};
    signal.aborted = true;
    signal.reason = reason;
    return signal;
}


void
dom::aborting::abort_signal::throw_if_aborted() {

    helpers::exceptions::throw_v8_exception(
            ext::property_any_cast<other::dom_exception>(reason).message,
            ABORT_ERR,
            [this] {return reason->has_value();});
}
