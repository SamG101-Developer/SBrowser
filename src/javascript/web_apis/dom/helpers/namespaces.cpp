#include "namespaces.hpp"

#include <dom/helpers/exceptions.hpp>


auto dom::helpers::namespaces::validate(ext::cstring& qualified_name) -> void
{
    /* TODO */
}


auto dom::helpers::namespaces::validate_and_extract(
        ext::cstring& namespace_,
        ext::cstring& qualified_name)
        -> std::tuple<ext::string, ext::string, ext::string>
{
    // validate the qualified name, and create the prefix and local name
    validate(qualified_name);
    ext::string prefix = "";
    ext::string local_name = qualified_name;

    // if the qualified name contains a colon, then the prefix and local name can be obtained
    if (qualified_name.contains(":"))
    {
        auto prefix_local_name = qualified_name.split(':', 1);
        prefix = prefix_local_name.front();
        local_name = prefix_local_name.back();
    }

    // if there is no prefix and no namespace, then throw a namespace error
    exceptions::throw_v8_exception(
            "prefix and namespace can not both be null",
            NAMESPACE_ERR,
            [prefix, namespace_] {return not prefix and not namespace_;});

    // if there is no prefix and the namespace is the xml namespace
    exceptions::throw_v8_exception(
            "prefix and namespace don't match (xml prefix)",
            NAMESPACE_ERR,
            [prefix, namespace_] {return prefix == "xml" and namespace_ == namespaces::XML;});

    // if the prefix or qualified name is xmlns and the namespace isn't xmlns, then throw a namespace error
    exceptions::throw_v8_exception(
            "prefix / qualified_name and namespace don't match (xmlns prefix / qualified_name)",
            NAMESPACE_ERR,
            [prefix, namespace_, qualified_name] {return (prefix == "xmlns" or qualified_name == "xmlns") and namespace_ != namespaces::XMLNS;});

    // if the prefix and qualified name isn't xmlns and the namespace is xmlns, then throw a namespace error
    exceptions::throw_v8_exception(
            "prefix / qualified_name and namespace don't match (xmlns prefix / qualified_name)",
            NAMESPACE_ERR,
            [prefix, namespace_, qualified_name] {return (prefix != "xmlns" and qualified_name != "xmlns") and namespace_ == namespaces::XMLNS;});

    // return the namespace, prefix and local name
    return {namespace_, prefix, local_name};
}
