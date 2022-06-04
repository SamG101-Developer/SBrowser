#pragma once
#ifndef SBROWSER_ABORT_SIGNAL_HPP
#define SBROWSER_ABORT_SIGNAL_HPP

#include <dom/nodes/event_target.hpp>

namespace dom::aborting {class abort_signal;}

#include <v8.h>
#include <veque.hpp>

namespace dom::helpers {struct event_listening;}
namespace dom::helpers {struct signal_aborting;}


class dom::aborting::abort_signal final: public nodes::event_target
{
public friends:
    friend class helpers::event_listening;
    friend class helpers::signal_aborting;

public aliases:
    using abort_signal_callback_t  = std::function<void()>;
    using abort_signal_callbacks_t = ext::vector<abort_signal_callback_t>;

public constructors:
    abort_signal();

public js_methods:
    new_obj static auto abort(ext::any reason = ext::string{""}) -> abort_signal;
    new_obj static auto timeout(ulonglong milliseconds) -> abort_signal;
    auto throw_if_aborted() const -> void;

public js_properties:
    ext::property<bool    > aborted;
    ext::property<ext::any> reason;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private cpp_properties:
    abort_signal_callbacks_t m_abort_algorithms;

};


#endif //SBROWSER_ABORT_SIGNAL_HPP
