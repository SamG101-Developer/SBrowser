#include "abort_controller.hpp"

#include <dom/aborting/abort_signal.hpp>
#include <dom/helpers/abort.hpp>

dom::aborting::abort_controller::abort_controller():
    signal(new abort_signal{}) {
}


dom::aborting::abort_controller::~abort_controller() {
    delete signal;
}


void dom::aborting::abort_controller::abort() {
    aborting::signal_abort(signal);
}
