#pragma once
#ifndef SBROWSER_ABORT_CONTROLLER_HPP
#define SBROWSER_ABORT_CONTROLLER_HPP

#include <ext/macros/decorators.hpp>
#include <ext/properties/dom_property.hpp>
#include <dom_object.hpp>

namespace dom::aborting
{
    class abort_controller;
    class abort_signal;
}


/*
 * https://dom.spec.whatwg.org/#interface-AbortSignal
 * https://developer.mozilla.org/en-US/docs/Web/API/AbortController
 *
 * The AbortController interface represents a controller object that allows you to abort one or more Web requests as and
 * when desired. You can create a new AbortController object using the AbortController() constructor. Communicating with
 * a DOM request is done using an AbortSignal object.
 */
class dom::aborting::abort_controller final : virtual public dom_object
{
public constructors:
    abort_controller();

public methods:
    auto abort(ext::cany& reason) const -> void;

public properties:
    ext::dom_property<abort_signal*, _F> signal;

public internal_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_ABORT_CONTROLLER_HPP
