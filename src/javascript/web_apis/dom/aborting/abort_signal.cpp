#include "abort_signal.hpp"

#include <dom/helpers/exceptions.hpp>


dom::aborting::abort_signal::abort_signal() : nodes::event_target()
{
}


dom::aborting::abort_signal
dom::aborting::abort_signal::abort(ext::any reason)
{
    // create the reason as an abort error if the reason isn't given
    reason = not reason.empty()
            ? reason
            : ext::any{other::dom_exception{"", ABORT_ERR}};

    // create a pre-aborted signal with the reason, and return the signal
    abort_signal signal{};
    signal.aborted = true;
    signal.reason = reason;
    return signal;
}


auto dom::aborting::abort_signal::throw_if_aborted() const -> void
{
    // if a reason has been given, then throw an abort error
    helpers::exceptions::throw_v8_exception(
            ext::property_any_cast<other::dom_exception>(reason).message,
            ABORT_ERR,
            [this] {return reason->to<bool>();});
}


auto dom::aborting::abort_signal::timeout(unsigned long long milliseconds) -> dom::aborting::abort_signal
{
    abort_signal signal{};
    // TODO
    return signal;
}


auto dom::aborting::abort_signal::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<abort_signal>{isolate}
            .inherit<event_target>()
            .static_("timeout", &abort_signal::timeout)
            .function("abort", &abort_signal::abort)
            .function("throwIfAborted", &abort_signal::throw_if_aborted)
            .var("aborted", &abort_signal::aborted)
            .var("reason", &abort_signal::reason)
            .auto_wrap_objects();
}
