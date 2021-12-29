#pragma once
#ifndef SBROWSER_ABORT_CONTROLLER_HPP
#define SBROWSER_ABORT_CONTROLLER_HPP

#include <ext/decorators.hpp>
#include <ext/property.hpp>

namespace dom::aborting {
    class abort_controller;
    class abort_signal;
}


class dom::aborting::abort_controller {
public constructors:
    abort_controller();
    ~abort_controller();

public methods:
    void abort(ext::any&& reason);

public properties:
    ext::dom_property<abort_signal*> signal;
};


#endif //SBROWSER_ABORT_CONTROLLER_HPP
