#pragma once
#ifndef SBROWSER_ABORT_SIGNAL_HPP
#define SBROWSER_ABORT_SIGNAL_HPP

#include <ext/dom_property.hpp>
#include <dom/nodes/event_target.hpp>

#include <v8.h>
#include <veque.hpp>

namespace dom::aborting {class abort_signal;}
namespace dom::helpers {struct event_listening;}
namespace dom::helpers {struct signal_aborting;}


class dom::aborting::abort_signal final: public nodes::event_target
{
public friends:
    friend class helpers::event_listening;
    friend class helpers::signal_aborting;

public aliases:
    using abort_signal_callback  = std::function<void()>;
    using abort_signal_callbacks = ext::vector<abort_signal_callback>;

public constructors:
    abort_signal();

public methods:
    new_obj static auto abort(ext::any reason = "") -> abort_signal;
    new_obj static auto timeout(unsigned long long milliseconds) -> abort_signal;
    auto throw_if_aborted() const -> void;

public properties:
    ext::dom_property<bool    , _F> aborted;
    ext::dom_property<ext::any, _F> reason;

public internal_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private internal_properties:
    abort_signal_callbacks m_abort_algorithms;

};


#endif //SBROWSER_ABORT_SIGNAL_HPP
