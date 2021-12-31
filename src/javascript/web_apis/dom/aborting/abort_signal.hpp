#pragma once
#ifndef SBROWSER_ABORT_SIGNAL_HPP
#define SBROWSER_ABORT_SIGNAL_HPP

#include <ext/iterables/set.hpp>
#include <ext/types/property.hpp>
#include <dom/nodes/event_target.hpp>

namespace dom {
    namespace aborting {class abort_signal;}
    namespace helpers {
        class event_listening;
        class signal_aborting;
    }
}


class dom::aborting::abort_signal : public nodes::event_target {

friends
    friend class helpers::event_listening;
    friend class helpers::signal_aborting;

aliases
    using abort_signal_callback  = std::function<void()>;
    using abort_signal_callbacks = ext::vector<abort_signal_callback>;

public constructors:
    abort_signal();

public methods:
    static abort_signal abort(ext::any reason = "");
    void throw_if_aborted();

public properties:
    ext::dom_property<bool> aborted;
    ext::dom_property<ext::any> reason;

protected internal_properties:
    abort_signal_callbacks m_abort_algorithms;

};


#endif //SBROWSER_ABORT_SIGNAL_HPP
