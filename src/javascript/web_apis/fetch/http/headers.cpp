#include "headers.hpp"

#include <dom/helpers/exceptions.hpp>
#include <fetch/helpers/header_internals.hpp>


auto fetch::http::headers::append(
        const ext::string& name,
        ext::string& value)
        -> void
{
    // normalize the value by trimming it TODO : move into own method in helpers
    value.trim(' ');

    // if the name and value aren't valid, then throw a type error
    dom::helpers::exceptions::throw_v8_exception<V8_TYPE_ERROR>(
            "name and value must be valid header-names and header-values respectively",
            [] {return false;}); // TODO

    // if the header guard is 'immutable', then throw type error
    dom::helpers::exceptions::throw_v8_exception<V8_TYPE_ERROR>(
            "header guard must not be 'immutable'",
            [this] {return m_header_guard == header_guard_t::IMMUTABLE;});

    // handle any checks depending on the header guard status and if the name is forbidden or not
    if (m_header_guard == header_guard_t::REQUEST and helpers::header_internals::is_forbidden_name(name))
        // return if the header guard is 'request' and the name is forbidden
        return;

    else if (m_header_guard == header_guard_t::RESPONSE and helpers::header_internals::is_forbidden_name(name))
        // return if the header guard is 'response' and the name is forbidden
        return;

    else if (m_header_guard == header_guard_t::REQUEST_NO_CORS)
    {
        auto temporary_value = helpers::header_internals::get(name, m_header_list).value_or(value);
        // TODO
    }

    // once the checks have passed, append the (name, value) pair to the headers
    helpers::header_internals::append({name, value}, m_header_list);

    // TODO : cors
}


auto fetch::http::headers::delete_(
        const ext::string& name)
        -> void
{

}
