#pragma once
#ifndef SBROWSER_DOM_EXCEPTION_HPP
#define SBROWSER_DOM_EXCEPTION_HPP

#include <ext/iterables/string.hpp>
#include <ext/macros/decorators.hpp>
#include <ext/types/property.hpp>

namespace dom::other {class dom_exception;}

enum exception_type {
    INDEX_SIZE_ERR, DOMSTRING_SIZE_ERR, HIERARCHY_REQUEST_ERR, WRONG_DOCUMENT_ERR, INVALID_CHARACTER_ERR,
    NO_DATA_ALLOWED_ERR, NO_MODIFICATION_ALLOWED_ERR, NOT_FOUND_ERR, NOT_SUPPORTED_ERR, INUSE_ATTRIBUTE_ERR,
    INVALID_STATE_ERR, SYNTAX_ERR, INVALID_MODIFICATION_ERR, NAMESPACE_ERR, INVALID_ACCESS_ERR, VALIDATION_ERR,
    TYPE_MISMATCH_ERR, SECURITY_ERR, NETWORK_ERR, ABORT_ERR, URL_MISMATCH_ERR, QUOTA_EXCEEDED_ERR, TIMEOUT_ERR,
    INVALID_NODE_TYPE_ERR, DATA_CLONE_ERR, ENCODING_ERR, NOT_READABLE_ERR, UNKNOWN_ERR, CONSTRAINT_ERR, DATA_ERR,
    TRANSACTION_INACTIVE_ERR, READONLY_ERR, VERSION_ERR, OPERATION_ERR, NOT_ALLOWED_ERR
};


class dom::other::dom_exception {
public: constructors
    dom_exception(ext::cstring& message, exception_type type);

public: properties
    ext::dom_property<ext::string> message;
    ext::dom_property<exception_type> type;
};


#endif //SBROWSER_DOM_EXCEPTION_HPP
