#include "namespaces.hpp"

#include <dom/helpers/exceptions.hpp>


void dom::helpers::namespaces::validate(ext::cstring& qualified_name) {/* TODO */}

std::tuple<ext::string, ext::string, ext::string>
dom::helpers::namespaces::validate_and_extract(
        ext::cstring& namespace_,
        ext::cstring& qualified_name) {

    validate(qualified_name);
    ext::string prefix = "";
    ext::string local_name = qualified_name;

    if (qualified_name.contains(*":")) {
        prefix = qualified_name.substring(0, qualified_name.find(*":"));
        local_name = qualified_name.substring(qualified_name.find(*":"), qualified_name.length());
    }

    exceptions::throw_v8_exception(
            "prefix and namespace can not both be null",
            NAMESPACE_ERR,
            [prefix, namespace_] {return not prefix and not namespace_;});

    exceptions::throw_v8_exception(
            "prefix and namespace don't match (xml prefix)",
            NAMESPACE_ERR,
            [prefix, namespace_] {return prefix == "xml" and namespace_ == namespaces::XML;});

    exceptions::throw_v8_exception(
            "prefix / qualified_name and namespace don't match (xmlns prefix / qualified_name)",
            NAMESPACE_ERR,
            [prefix, namespace_, qualified_name] {return (prefix == "xmlns" or qualified_name == "xmlns") and namespace_ != namespaces::XMLNS;});

    exceptions::throw_v8_exception(
            "prefix / qualified_name and namespace don't match (xmlns prefix / qualified_name)",
            NAMESPACE_ERR,
            [prefix, namespace_, qualified_name] {return (prefix != "xmlns" and qualified_name != "xmlns") and namespace_ == namespaces::XMLNS;});

    return {namespace_, prefix, local_name};
}
