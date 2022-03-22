#include "abort_signal.hpp"

#include <dom/helpers/exceptions.hpp>


dom::aborting::abort_signal::abort_signal() : nodes::event_target()
{
}


auto dom::aborting::abort_signal::abort(
        ext::any reason)
        -> dom::aborting::abort_signal
{
    // let signal be a new AbortSignal object.
    abort_signal signal{};

    // set signal’s abort reason to reason if it is given; otherwise to a new "AbortError" DOMException.
    reason = not reason.empty() ? reason : other::dom_exception{"", ABORT_ERR};

    signal.aborted = true;
    signal.reason = reason;

    // return signal.
    return signal;
}


auto dom::aborting::abort_signal::throw_if_aborted(
        ) const
        -> void
{
    // if a reason has been given, then throw an abort error
    helpers::exceptions::throw_v8_exception(
            ext::property_any_cast<other::dom_exception>(reason).message,
            ABORT_ERR,
            [this] {return reason->to<bool>();});
}


auto dom::aborting::abort_signal::timeout(
        const unsigned long long milliseconds)
        -> dom::aborting::abort_signal
{
    abort_signal signal{};
    // TODO
    return signal;
}


auto dom::aborting::abort_signal::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<abort_signal>{isolate}
            .template inherit<event_target>()
            .static_("timeout", &abort_signal::timeout)
            .template function("abort", &abort_signal::abort)
            .template function("throwIfAborted", &abort_signal::throw_if_aborted)
            .template var("aborted", &abort_signal::aborted, true)
            .template var("reason", &abort_signal::reason)
            .auto_wrap_objects();
}
