#include "abort_signal.hpp"

#include <dom/helpers/exceptions.hpp>
#include <javascript/environment/realms.hpp>


dom::aborting::abort_signal::abort_signal()
        : aborted(false)
        , reason(ext::string{})
{
}


auto dom::aborting::abort_signal::abort(
        ext::any reason)
        -> dom::aborting::abort_signal
{
    // create a signal
    abort_signal signal{};

    // set the signal's reason to an exception if not given
    reason = not reason.empty() ? reason : other::dom_exception{"", ABORT_ERR};
    signal.aborted = true;
    signal.reason  = reason;

    // return signal.
    return signal;
}


auto dom::aborting::abort_signal::throw_if_aborted() const
        -> void
{
    // if a reason has been given, then throw an abort error
    helpers::exceptions::throw_v8_exception<ABORT_ERR>(
            p_any_cast<other::dom_exception&>(reason)->message,
            [this] {return reason->to<bool>();});
}


auto dom::aborting::abort_signal::timeout(
        const ulonglong milliseconds)
        -> dom::aborting::abort_signal
{
    abort_signal signal{};
    auto global_object = javascript::realms::relevant_realm(&signal); // TODO : html spec timeout stuff
    return signal;
}


auto dom::aborting::abort_signal::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<abort_signal>{isolate}
            .inherit<event_target>()
            .function("timeout", &abort_signal::timeout)
            .function("abort", &abort_signal::abort)
            .function("throwIfAborted", &abort_signal::throw_if_aborted)
            .var("aborted", &abort_signal::aborted, true)
            .var("reason", &abort_signal::reason)
            .auto_wrap_objects();
}
