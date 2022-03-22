#pragma once
#ifndef SBROWSER_SIGNAL_ABORTING_HPP
#define SBROWSER_SIGNAL_ABORTING_HPP

#include <ext/any.hpp>

namespace dom::aborting {class abort_signal;}
namespace dom::helpers {struct signal_aborting;}


struct dom::helpers::signal_aborting final
{
    // other general helper methods for aborting
    static auto signal_abort(
            aborting::abort_signal* signal,
            const ext::any& reason) -> void;

    static auto follow_signal(
            aborting::abort_signal* following_signal,
            aborting::abort_signal* parent_signal) -> void;
};


#endif //SBROWSER_SIGNAL_ABORTING_HPP
