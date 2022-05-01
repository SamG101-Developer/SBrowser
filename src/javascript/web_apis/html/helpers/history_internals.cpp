#include "history_internals.hpp"

#include <dom/nodes/document.hpp>

#include <html/helpers/browsing_context_internals.hpp>


auto html::helpers::history_internals::url_and_history_update_steps(
        dom::nodes::document* document,
        url::url& new_url,
        const ext::string& serialized_data,
        bool is_push)
        -> void
{
    auto* browsing_context = document->m_browsing_context;
    if (document->m_is_initial)
        is_push = false;

    if (is_push)
    {
        browsing_context->joint_session_history.remove_all_after(browsing_context->current_entry_joint_session_history);
        // TODO
        // TODO
        browsing_context->joint_session_history.insert(
                new internal::session_history_entry
                {
                    .url_record=new_url,
                    .document=document,
                    .serialized_state=serialized_data,
                    .scroll_restoration_mode=browsing_context->current_entry_joint_session_history->scroll_restoration_mode
                });
    }
}
