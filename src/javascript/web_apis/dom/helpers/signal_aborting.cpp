#include "signal_aborting.hpp"

#include <dom/aborting/abort_signal.hpp>
#include <dom/helpers/event_dispatching.hpp>


void
dom::helpers::signal_aborting::signal_abort(
        aborting::abort_signal* signal,
        ext::cany& reason) {

    if (signal->aborted) return;

    signal->aborted = true;
    signal->reason = reason;
    signal->m_abort_algorithms.for_each([](auto& algorithm) {algorithm();});
    signal->m_abort_algorithms.clear();

    event_dispatching::fire_event<>("Abort", signal);
}


void
dom::helpers::signal_aborting::follow_signal(
        aborting::abort_signal* following_signal,
        aborting::abort_signal* parent_signal) {

    if (following_signal->aborted)
        return;

    if (parent_signal->aborted)
        signal_abort(following_signal, parent_signal->reason);

    else
        parent_signal->m_abort_algorithms.append([following_signal, parent_signal]() {signal_abort(following_signal, parent_signal->reason);});
}
