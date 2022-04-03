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
        m_lazy_load_resumption_steps = std::function{[] {}};
    }

    loading << val;
}


auto html::mixins::lazy_loading::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<lazy_loading>{isolate}
            .inherit<dom_object>()
            .var("loading", &lazy_loading::loading)
            .auto_wrap_objects();
}

