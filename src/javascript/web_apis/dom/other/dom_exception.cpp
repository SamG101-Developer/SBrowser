#include "dom_exception.hpp"


dom::other::dom_exception::dom_exception(
        ext::cstring& message,
        exception_type type)

        : dom_object() {

    this->message = message;
    this->type = type;
}


ext::any
dom::other::dom_exception::v8(v8::Isolate* isolate) const {

    return v8pp::class_<dom_exception>{isolate}
            .ctor<ext::cstring&, exception_type>()
            .var("message", &dom::other::dom_exception::message)
            .var("type", &dom::other::dom_exception::type)

            .static_("INDEX_SIZE_ERR", exception_type::INDEX_SIZE_ERR)
            .static_("DOMSTRING_SIZE_ERR", exception_type::DOMSTRING_SIZE_ERR)
            .static_("HIERARCHY_REQUEST_ERR", exception_type::HIERARCHY_REQUEST_ERR)
            .static_("WRONG_DOCUMENT_ERR", exception_type::WRONG_DOCUMENT_ERR)
            .static_("INVALID_CHARACTER_ERR", exception_type::INVALID_CHARACTER_ERR)
            .static_("NO_DATA_ALLOWED_ERR", exception_type::NO_DATA_ALLOWED_ERR)
            .static_("NO_MODIFICATION_ALLOWED_ERR", exception_type::NO_MODIFICATION_ALLOWED_ERR)
            .static_("NOT_FOUND_ERR", exception_type::NOT_FOUND_ERR)
            .static_("NOT_SUPPORTED_ERR", exception_type::NOT_SUPPORTED_ERR)
            .static_("INUSE_ATTRIBUTE_ERR", exception_type::INUSE_ATTRIBUTE_ERR)
            .static_("INVALID_STATE_ERR", exception_type::INVALID_STATE_ERR)
            .static_("SYNTAX_ERR", exception_type::SYNTAX_ERR)
            .static_("INVALID_MODIFICATION_ERR", exception_type::INVALID_MODIFICATION_ERR)
            .static_("NAMESPACE_ERR", exception_type::NAMESPACE_ERR)
            .static_("INVALID_ACCESS_ERR", exception_type::INVALID_ACCESS_ERR)
            .static_("VALIDATION_ERR", exception_type::VALIDATION_ERR)
            .static_("TYPE_MISMATCH_ERR", exception_type::TYPE_MISMATCH_ERR)
            .static_("SECURITY_ERR", exception_type::SECURITY_ERR)
            .static_("NETWORK_ERR", exception_type::NETWORK_ERR)
            .static_("ABORT_ERR", exception_type::ABORT_ERR)
            .static_("URL_MISMATCH_ERR", exception_type::URL_MISMATCH_ERR)
            .static_("QUOTA_EXCEEDED_ERR", exception_type::QUOTA_EXCEEDED_ERR)
            .static_("TIMEOUT_ERR", exception_type::TIMEOUT_ERR)
            .static_("INVALID_NODE_TYPE_ERR", exception_type::INVALID_NODE_TYPE_ERR)
            .static_("DATA_CLONE_ERR", exception_type::DATA_CLONE_ERR)
            .static_("ENCODING_ERR", exception_type::ENCODING_ERR)
            .static_("NOT_READABLE_ERR", exception_type::NOT_READABLE_ERR)
            .static_("UNKNOWN_ERR", exception_type::UNKNOWN_ERR)
            .static_("CONSTRAINT_ERR", exception_type::CONSTRAINT_ERR)
            .static_("DATA_ERR", exception_type::DATA_ERR)
            .static_("TRANSACTION_INACTIVE_ERR", exception_type::TRANSACTION_INACTIVE_ERR)
            .static_("READONLY_ERR", exception_type::READONLY_ERR)
            .static_("VERSION_ERR", exception_type::VERSION_ERR)
            .static_("OPERATION_ERR", exception_type::OPERATION_ERR)
            .static_("NOT_ALLOWED_ERR", exception_type::NOT_ALLOWED_ERR)

            .auto_wrap_objects();
}
