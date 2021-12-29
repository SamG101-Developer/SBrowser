#pragma once
#ifndef SBROWSER_SIGNAL_ABORTING_HPP
#define SBROWSER_SIGNAL_ABORTING_HPP

#include <ext/any.hpp>

namespace dom {
    namespace helpers {struct signal_aborting;}
    namespace aborting {class abort_signal;}
}


struct dom::helpers::signal_aborting {
    static void signal_abort(aborting::abort_signal* signal, ext::cany& reason);
    static void follow_signal(aborting::abort_signal* following_signal, aborting::abort_signal* parent_signal);
};


#endif //SBROWSER_SIGNAL_ABORTING_HPP
