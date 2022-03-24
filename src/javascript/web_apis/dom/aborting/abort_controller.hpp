#pragma once
#ifndef SBROWSER_ABORT_CONTROLLER_HPP
#define SBROWSER_ABORT_CONTROLLER_HPP

#include <ext/decorators.hpp>
#include <ext/dom_property.hpp>
#include <dom_object.hpp>

namespace dom::aborting {class abort_controller;}
namespace dom::aborting {class abort_signal;}


class dom::aborting::abort_controller final : virtual public dom_object
{
public constructors:
    abort_controller();

public methods:
    auto abort(const ext::any& reason) const -> void;

public properties:
    ext::dom_property<abort_signal*> signal;

public internal_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_ABORT_CONTROLLER_HPP
