#pragma once
#ifndef SBROWSER_DOM_EXCEPTION_HPP
#define SBROWSER_DOM_EXCEPTION_HPP

#include <ext/iterables/string.hpp>
#include <ext/macros/decorators.hpp>
#include <ext/properties/dom_property.hpp>

#include <dom_object.hpp>

namespace dom::other {class dom_exception;}

enum v8_custom_error_t {
    INDEX_SIZE_ERR, DOMSTRING_SIZE_ERR, HIERARCHY_REQUEST_ERR, WRONG_DOCUMENT_ERR, INVALID_CHARACTER_ERR,
    NO_DATA_ALLOWED_ERR, NO_MODIFICATION_ALLOWED_ERR, NOT_FOUND_ERR, NOT_SUPPORTED_ERR, INUSE_ATTRIBUTE_ERR,
    INVALID_STATE_ERR, SYNTAX_ERR, INVALID_MODIFICATION_ERR, NAMESPACE_ERR, INVALID_ACCESS_ERR, VALIDATION_ERR,
    TYPE_MISMATCH_ERR, SECURITY_ERR, NETWORK_ERR, ABORT_ERR, URL_MISMATCH_ERR, QUOTA_EXCEEDED_ERR, TIMEOUT_ERR,
    INVALID_NODE_TYPE_ERR, DATA_CLONE_ERR, ENCODING_ERR, NOT_READABLE_ERR, UNKNOWN_ERR, CONSTRAINT_ERR, DATA_ERR,
    TRANSACTION_INACTIVE_ERR, READONLY_ERR, VERSION_ERR, OPERATION_ERR, NOT_ALLOWED_ERR
};


class dom::other::dom_exception : public virtual dom_object {
public: constructors
    dom_exception() = default;
    dom_exception(ext::cstring& message, v8_custom_error_t type);

public: properties
    ext::dom_property<ext::string, _F> message;
    ext::dom_property<v8_custom_error_t, _F> type;

public: internal_methods
    ext::any v8(v8::Isolate *isolate) const override;
};


#endif //SBROWSER_DOM_EXCEPTION_HPP
