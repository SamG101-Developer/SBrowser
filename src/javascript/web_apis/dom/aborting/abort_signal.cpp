#include "abort_signal.hpp"


dom::aborting::abort_signal::abort_signal()
        : nodes::event_target() {
}


dom::aborting::abort_signal
dom::aborting::abort_signal::abort() {

    abort_signal signal{};
    signal.aborted = true;
    return signal;
}
