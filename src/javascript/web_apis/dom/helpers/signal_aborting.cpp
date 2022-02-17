#include "signal_aborting.hpp"

#include <dom/aborting/abort_signal.hpp>
#include <dom/helpers/event_dispatching.hpp>


void
dom::helpers::signal_aborting::signal_abort(
        aborting::abort_signal* signal,
        ext::cany& reason)
{
    // return if the signal ahs already been aborted
    if (signal->aborted)
        return;

    // set the signal to be aborted (with a possible reason), call all the abort algorithms, and clear the set of abort
    // algorithms
    signal->aborted = true;
    signal->reason = reason;
    signal->m_abort_algorithms.call_all();
    signal->m_abort_algorithms.clear();

    // fire an abort event at the signal
    event_dispatching::fire_event<>("Abort", signal);
}


void
dom::helpers::signal_aborting::follow_signal(
        aborting::abort_signal* following_signal,
        aborting::abort_signal* parent_signal)
{
    // return if the following signal has aborted
    if (following_signal->aborted)
        return;

    // signal abort the following signal with the parent signal reason if the parent signal has already aborted
    if (parent_signal->aborted)
        signal_abort(following_signal, parent_signal->reason);

    // otherwise, append an abort algorithm to the parent signal that will abort the following signal with the parent
    // signal's reason, so when the parent signal aborts, so does the following signal
    else
        parent_signal->m_abort_algorithms.append([&following_signal, &parent_signal]() {signal_abort(following_signal, parent_signal->reason);});
}
