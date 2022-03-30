#include "lazy_loading.hpp"

#include <ext/equalities.hpp>


html::mixins::lazy_loading::lazy_loading() = default;


auto html::mixins::lazy_loading::set_loading(
        const ext::string& val)
        -> void
{
    // special steps for the eager value being set
    if (val == "eager")
    {
        // return if the lazy load resumption steps are empty ie no steps
        if (m_lazy_load_resumption_steps == std::function{[] {}}) return;

        // invoke the lazy load and clear them afterwards
        m_lazy_load_resumption_steps();
        m_lazy_load_resumption_steps = [] {};
    }

    loading << val;
}

