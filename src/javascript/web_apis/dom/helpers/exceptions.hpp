#pragma once
#ifndef SBROWSER_EXCEPTIONS_HPP
#define SBROWSER_EXCEPTIONS_HPP

#define empty_conditional [](){return true;}

#include <functional>
#include <ext/string.hpp>

namespace dom::helpers {
    struct exceptions;
}


struct dom::helpers::exceptions {
    using exception_condiditional = std::function<bool()>&&;
    enum exception_type;

    static void throw_v8_exception(ext::cstring& exception_message, exception_type exception_type, exception_condiditional conditional = empty_conditional);
};


enum dom::helpers::exceptions::exception_type {
    INDEX_SIZE_ERR, DOMSTRING_SIZE_ERR, HIERARCHY_REQUEST_ERR, WRONG_DOCUMENT_ERR, INVALID_CHARACTER_ERR,
    NO_DATA_ALLOWED_ERR, NO_MODIFICATION_ALLOWED_ERR, NOT_FOUND_ERR, NOT_SUPPORTED_ERR, INUSE_ATTRIBUTE_ERR,
    INVALID_STATE_ERR, SYNTAX_ERR, INVALID_MODIFICATION_ERR, NAMESPACE_ERR, INVALID_ACCESS_ERR, VALIDATION_ERR,
    TYPE_MISMATCH_ERR, SECURITY_ERR, NETWORK_ERR, ABORT_ERR, URL_MISMATCH_ERR, QUOTA_EXCEEDED_ERR, TIMEOUT_ERR,
    INVALID_NODE_TYPE_ERR, DATA_CLONE_ERR, ENCODING_ERR, NOT_READABLE_ERR, UNKNOWN_ERR, CONSTRAINT_ERR, DATA_ERR,
    TRANSACTION_INACTIVE_ERR, READONLY_ERR, VERSION_ERR, OPERATION_ERR, NOT_ALLOWED_ERR
};


#endif //SBROWSER_EXCEPTIONS_HPP
