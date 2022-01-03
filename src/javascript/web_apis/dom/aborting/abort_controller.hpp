#pragma once
#ifndef SBROWSER_ABORT_CONTROLLER_HPP
#define SBROWSER_ABORT_CONTROLLER_HPP

#include <ext/macros/decorators.hpp>
#include <ext/types/property.hpp>

#include <dom_object.hpp>

namespace dom::aborting {
    class abort_controller;
    class abort_signal;
}


class dom::aborting::abort_controller final : public dom_object {
public constructors:
    abort_controller();
    ~abort_controller() override;

public methods:
    void abort(ext::any&& reason);

public properties:
    ext::dom_property<abort_signal*> signal;

public: internal_methods
    ext::any v8(v8::Isolate* isolate) const override;
};


#endif //SBROWSER_ABORT_CONTROLLER_HPP
