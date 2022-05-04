#include "location.hpp"

#include <javascript/environment/realms.hpp>

#include <dom/nodes/document.hpp>
#include <dom/nodes/window.hpp>

#include <html/helpers/browsing_context_internals.hpp>


html::other::location::location()
{
    // set the custom accessor methods
    href.getter      = [this] {return get_href();};
    origin.getter    = [this] {return get_origin();};
    protocol.getter  = [this] {return get_protocol();};
    host.getter      = [this] {return get_host();};
    hostname.getter  = [this] {return get_hostname();};
    port.getter      = [this] {return get_port();};
    pathname.getter  = [this] {return get_pathname();};
    search.getter    = [this] {return get_search();};
    hash.getter      = [this] {return get_hash();};

    // set the attribute values
    m_relevant_document = javascript::realms::relevant_realm(this).global_object->document->m_browsing_context->active_document(); // TODO : handle null context
}
