#include "abort_signal.hpp"

#include <dom/helpers/exceptions.hpp>


dom::aborting::abort_signal::abort_signal() : nodes::event_target()
{
}


/*
 * https://dom.spec.whatwg.org/#dom-abortsignal-abort
 * https://developer.mozilla.org/en-US/docs/Web/API/AbortSignal/abort
 *
 * The static AbortSignal.abort() method returns an AbortSignal that is already set as aborted (and which does not
 * trigger an abort event). This could, for example, be passed to a fetch method in order to run its abort logic (i.e.
 * it may be that code is organized such that the abort logic should be run even if the intended fetch operation has not
 * been started). Note: The method is similar in purpose to Promise.reject.
 */
auto dom::aborting::abort_signal::abort(ext::any reason) -> dom::aborting::abort_signal
{
    // Let signal be a new AbortSignal object.
    abort_signal signal{};

    // Set signal’s abort reason to reason if it is given; otherwise to a new "AbortError" DOMException.
    reason = not reason.empty()
            ? reason
            : ext::any{other::dom_exception{"", ABORT_ERR}};

    signal.aborted = true;
    signal.reason = reason;

    // Return signal.
    return signal;
}


/*
 * https://dom.spec.whatwg.org/#dom-abortsignal-throwifaborted
 * https://developer.mozilla.org/en-US/docs/Web/API/AbortSignal/throwIfAborted
 *
 * The throwIfAborted() method throws the signal's abort reason if the signal has been aborted; otherwise it does
 * nothing. An API that needs to support aborting can accept an AbortSignal object and use throwIfAborted() to test and
 * throw when the abort event is signalled. This method can also be used to abort operations at particular points in
 * code, rather than passing to functions that take a signal.
 */
auto dom::aborting::abort_signal::throw_if_aborted() const -> void
{
    // if a reason has been given, then throw an abort error
    helpers::exceptions::throw_v8_exception(
            ext::property_any_cast<other::dom_exception>(reason).message,
            ABORT_ERR,
            [this] {return reason->to<bool>();});
}


/*
 * https://dom.spec.whatwg.org/#dom-abortsignal-timeout
 */
auto dom::aborting::abort_signal::timeout(const unsigned long long milliseconds) -> dom::aborting::abort_signal
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
