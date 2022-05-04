#include "history_internals.hpp"

#include <dom/nodes/document.hpp>

#include <html/helpers/browsing_context_internals.hpp>

#include <url/url.hpp>


auto html::helpers::history_internals::url_and_history_update_steps(
        dom::nodes::document* document,
        url::url& new_url,
        const ext::string& serialized_data,
        bool is_push)
        -> void
{
    // get the browsing context of the document, and return if the document is in the initial state
    auto* browsing_context = document->m_browsing_context;
    if (document->m_is_initial)
        is_push = false;

    // handle the case of is_push being true
    if (is_push)
    {
        if (browsing_context->session_history.back() != browsing_context->current_entry)
            browsing_context->session_history.remove_all_after(browsing_context->current_entry);

        // TODO
        // TODO

        auto* new_entry = new internal::session_history_entry
        {
                .url_record=new_url,
                .document=document,
                .serialized_state=serialized_data,
                .scroll_restoration_mode=browsing_context->current_entry->scroll_restoration_mode
        }

        browsing_context->session_history.insert(new_entry, browsing_context->session_history.find(browsing_context->current_entry));
        browsing_context->current_entry = new_entry;
    }
    else
    {
        auto* entry = browsing_context->current_entry;
        entry->url_record = new_url;
        entry->serialized_state = serialized_data ? serialized_data : entry->serialized_state;
        // TODO
    }

    document->url = &new_url;
    // TODO
    browsing_context->current_entry->document->m_latest_entry = browsing_context->current_entry->document->m_browsing_context->current_entry;
}


auto html::helpers::history_internals::rewrite_url(
        dom::nodes::document* document,
        url::url& target_url)
        -> void
{
    url::url& document_url = document->url;
    if (document_url.scheme() != target_url.scheme() /* TODO : other checks */)
}
