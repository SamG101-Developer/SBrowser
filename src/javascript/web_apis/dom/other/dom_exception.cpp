#include "dom_exception.hpp"


dom::other::dom_exception::dom_exception(
        const ext::string& message,
        const v8_custom_error_t type)

        : dom_object()
{
    // set the property values
    this->message << message;
    this->type << type;
}


auto dom::other::dom_exception::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<dom_exception>{isolate}
            .ctor<const ext::string&, v8_custom_error_t>()
            .var("message", &dom::other::dom_exception::message)
            .var("type", &dom::other::dom_exception::type)
            .template static_("INDEX_SIZE_ERR", v8_custom_error_t::INDEX_SIZE_ERR)
            .template static_("DOMSTRING_SIZE_ERR", v8_custom_error_t::DOMSTRING_SIZE_ERR)
            .template static_("HIERARCHY_REQUEST_ERR", v8_custom_error_t::HIERARCHY_REQUEST_ERR)
            .template static_("WRONG_DOCUMENT_ERR", v8_custom_error_t::WRONG_DOCUMENT_ERR)
            .template static_("INVALID_CHARACTER_ERR", v8_custom_error_t::INVALID_CHARACTER_ERR)
            .template static_("NO_DATA_ALLOWED_ERR", v8_custom_error_t::NO_DATA_ALLOWED_ERR)
            .template static_("NO_MODIFICATION_ALLOWED_ERR", v8_custom_error_t::NO_MODIFICATION_ALLOWED_ERR)
            .template static_("NOT_FOUND_ERR", v8_custom_error_t::NOT_FOUND_ERR)
            .template static_("NOT_SUPPORTED_ERR", v8_custom_error_t::NOT_SUPPORTED_ERR)
            .template static_("INUSE_ATTRIBUTE_ERR", v8_custom_error_t::INUSE_ATTRIBUTE_ERR)
            .template static_("INVALID_STATE_ERR", v8_custom_error_t::INVALID_STATE_ERR)
            .template static_("SYNTAX_ERR", v8_custom_error_t::SYNTAX_ERR)
            .template static_("INVALID_MODIFICATION_ERR", v8_custom_error_t::INVALID_MODIFICATION_ERR)
            .template static_("NAMESPACE_ERR", v8_custom_error_t::NAMESPACE_ERR)
            .template static_("INVALID_ACCESS_ERR", v8_custom_error_t::INVALID_ACCESS_ERR)
            .template static_("VALIDATION_ERR", v8_custom_error_t::VALIDATION_ERR)
            .template static_("TYPE_MISMATCH_ERR", v8_custom_error_t::TYPE_MISMATCH_ERR)
            .template static_("SECURITY_ERR", v8_custom_error_t::SECURITY_ERR)
            .template static_("NETWORK_ERR", v8_custom_error_t::NETWORK_ERR)
            .template static_("ABORT_ERR", v8_custom_error_t::ABORT_ERR)
            .template static_("URL_MISMATCH_ERR", v8_custom_error_t::URL_MISMATCH_ERR)
            .template static_("QUOTA_EXCEEDED_ERR", v8_custom_error_t::QUOTA_EXCEEDED_ERR)
            .template static_("TIMEOUT_ERR", v8_custom_error_t::TIMEOUT_ERR)
            .template static_("INVALID_NODE_TYPE_ERR", v8_custom_error_t::INVALID_NODE_TYPE_ERR)
            .template static_("DATA_CLONE_ERR", v8_custom_error_t::DATA_CLONE_ERR)
            .template static_("ENCODING_ERR", v8_custom_error_t::ENCODING_ERR)
            .template static_("NOT_READABLE_ERR", v8_custom_error_t::NOT_READABLE_ERR)
            .template static_("UNKNOWN_ERR", v8_custom_error_t::UNKNOWN_ERR)
            .template static_("CONSTRAINT_ERR", v8_custom_error_t::CONSTRAINT_ERR)
            .template static_("DATA_ERR", v8_custom_error_t::DATA_ERR)
            .template static_("TRANSACTION_INACTIVE_ERR", v8_custom_error_t::TRANSACTION_INACTIVE_ERR)
            .template static_("READONLY_ERR", v8_custom_error_t::READONLY_ERR)
            .template static_("VERSION_ERR", v8_custom_error_t::VERSION_ERR)
            .template static_("OPERATION_ERR", v8_custom_error_t::OPERATION_ERR)
            .template static_("NOT_ALLOWED_ERR", v8_custom_error_t::NOT_ALLOWED_ERR)
            .auto_wrap_objects();
}
