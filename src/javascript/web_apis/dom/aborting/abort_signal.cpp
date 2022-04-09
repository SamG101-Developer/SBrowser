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
    helpers::exceptions::throw_v8_exception<ABORT_ERR>(
            ext::property_any_cast<other::dom_exception>(reason).message,
            [this] {return reason->to<bool>();});
}


auto dom::aborting::abort_signal::timeout(
        const ulong long milliseconds)
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
            .inherit<event_target>()
            .template static_("timeout", &abort_signal::timeout)
            .function("abort", &abort_signal::abort)
            .function("throwIfAborted", &abort_signal::throw_if_aborted)
            .var("aborted", &abort_signal::aborted, true)
            .var("reason", &abort_signal::reason)
            .auto_wrap_objects();
}
