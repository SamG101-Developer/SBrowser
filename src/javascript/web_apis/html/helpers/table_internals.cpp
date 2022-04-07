#include "table_internals.hpp"

#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/namespaces.hpp>


template <typename T>
auto html::helpers::table_internals::table_create(
        dom::nodes::document* document,
        const ext::string& local_name)
        -> T*
{
    // return the element created with the HTML namespace
    return dom::helpers::custom_elements::create_an_element<T>(document, local_name, dom::helpers::namespaces::HTML);
}
