#include "history.hpp"

#include <dom/helpers/exceptions.hpp>

#include <dom/nodes/document.hpp>

#include <html/helpers/browsing_context_internals.hpp>
#include <html/helpers/document_internals.hpp>
#include <html/helpers/history_internals.hpp>


html::other::history::history()
{
    // constrain the property values
    scroll_restoration.constrain_values({"auto", "normal"});

    // set the custom accessor methods
    length            .getter = [this] {return get_length();};
    scroll_restoration.getter = [this] {return get_scroll_restoration();};
    state             .getter = [this] {return get_state();};
}


auto html::other::history::go(
        long delta)
        -> void
{
    dom::helpers::exceptions::throw_v8_exception<SECURITY_ERR>(
            "document must be fully active",
            [this] {return not helpers::document_internals::is_active_document(m_document);});

    // TODO : delta == 0 : custom behaviour

    helpers::history_internals::travserse_history_by_delta(delta, m_document->m_browsing_context);
}


auto html::other::history::back()
        -> void
{
    dom::helpers::exceptions::throw_v8_exception<SECURITY_ERR>(
            "document must be fully active",
            [this] {return not helpers::document_internals::is_active_document(m_document);});

    helpers::history_internals::traverse_history_by_delta(-1, m_document->m_browsing_context);
}


auto html::other::history::forward()
        -> void
{
    dom::helpers::exceptions::throw_v8_exception<SECURITY_ERR>(
            "document must be fully active",
            [this] {return not helpers::document_internals::is_active_document(m_document);});

    helpers::history_internals::traverse_history_by_delta(1, m_document->m_browsing_context);
}


auto html::other::history::push_state(
        const ext::any& data,
        const ext::string& unused,
        const ext::string& url)
        -> void
{
    helpers::history_internals::shared_history_push_replace_state_steps(this, data, url, true);
}


auto html::other::history::replace_state(
        const ext::any& data,
        const ext::string& unused,
        const ext::string& url)
        -> void
{
    helpers::history_internals::shared_history_push_replace_state_steps(this, data, url, false);
}


auto html::other::history::get_length()
        -> ulong
{
    dom::helpers::exceptions::throw_v8_exception<SECURITY_ERR>(
            "document must be fully active",
            [this] {return not helpers::document_internals::is_active_document(m_document);});

    return m_document->m_browsing_context->joint_session_history.length();
}


auto html::other::history::get_scroll_restoration()
        -> ext::string
{
    dom::helpers::exceptions::throw_v8_exception<SECURITY_ERR>(
            "document must be fully active",
            [this] {return not helpers::document_internals::is_active_document(m_document);});

    return m_document->m_browsing_context->current_entry_joint_session_history->scroll_restoration_mode;
}


auto html::other::history::get_state()
        -> ext::any
{
    dom::helpers::exceptions::throw_v8_exception<SECURITY_ERR>(
            "document must be fully active",
            [this] {return not helpers::document_internals::is_active_document(m_document);});

    // TODO : return state internal value (wait for property class update 2.0 for this)
}


auto html::other::history::set_scroll_restoration(
        const ext::string& val)
        -> void
{
    dom::helpers::exceptions::throw_v8_exception<SECURITY_ERR>(
            "document must be fully active",
            [this] {return not helpers::document_internals::is_active_document(m_document);});

    m_document->m_browsing_context->current_entry_joint_session_history->scroll_restoration_mode = val;
}
