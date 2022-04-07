#include "namespaces.hpp"

#include <dom/helpers/exceptions.hpp>


auto dom::helpers::namespaces::validate(
        const ext::string& qualified_name)
        -> void
{
    // TODO : https://stackoverflow.com/a/51451972/10862918 for BNF impl
    exceptions::throw_v8_exception<INVALID_CHARACTER_ERR>(
            "qualified name doesn't match QName production",
            [qualified_name] {return xml_names::qualfied_names::match_qname();});
}


auto dom::helpers::namespaces::validate_and_extract(
        const ext::string& namespace_,
        const ext::string& qualified_name)
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
    exceptions::throw_v8_exception<NAMESPACE_ERR>(
            "prefix and namespace can not both be null",
            [prefix, namespace_] {return not prefix and not namespace_;});

    // if there is no prefix and the namespace is the xml namespace
    exceptions::throw_v8_exception<NAMESPACE_ERR>(
            "prefix and namespace don't match (xml prefix)",
            [prefix, namespace_] {return prefix == "xml" and namespace_ == namespaces::XML;});

    // if the prefix or qualified name is xmlns and the namespace isn't xmlns, then throw a namespace error
    exceptions::throw_v8_exception<NAMESPACE_ERR>(
            "prefix / qualified_name and namespace don't match (xmlns prefix / qualified_name)",
            [prefix, namespace_, qualified_name] {return (prefix == "xmlns" or qualified_name == "xmlns") and namespace_ != namespaces::XMLNS;});

    // if the prefix and qualified name isn't xmlns and the namespace is xmlns, then throw a namespace error
    exceptions::throw_v8_exception<NAMESPACE_ERR>(
            "prefix / qualified_name and namespace don't match (xmlns prefix / qualified_name)",
            [prefix, namespace_, qualified_name] {return (prefix != "xmlns" and qualified_name != "xmlns") and namespace_ == namespaces::XMLNS;});

    // return the namespace, prefix and local name
    return {namespace_, prefix, local_name};
}
