#include "headers.hpp"

#include <dom/helpers/exceptions.hpp>


auto fetch::http::headers::append(
        const ext::string& name,
        ext::string& value)
        -> void
{
        value.trim(' ');

        dom::helpers::exceptions::throw_v8_exception<V8_TYPE_ERROR>(
                "name and value must be valid header-names and header-values respectively",
                [] {return false;} /* TODO */);

        dom::helpers::exceptions::throw_v8_exception<V8_TYPE_ERROR>(
                "header guard must not be 'immutable'",
                [this] {return m_header_guard == "immutable";});
}
