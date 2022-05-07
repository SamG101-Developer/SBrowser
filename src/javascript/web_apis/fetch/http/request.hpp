#pragma once
#ifndef SBROWSER_REQUEST_HPP
#define SBROWSER_REQUEST_HPP

#include <dom_object.hpp>

namespace fetch::http {class request;}

#include <ext/string.hpp>
#include <url/url.hpp>
#include <javascript/environment/realms.hpp>

namespace dom::aborting {class abort_signal;}
namespace fetch::http {class headers;}
namespace html::internal {struct policy_container;}


class fetch::http::request : public virtual dom_object
{
public aliases:
    using request_info_t = std::variant<const ext::string&, request*>;

public constructors:
    request(request_info_t input, ext::string_any_map init = {});

public js_methods:
    new_obj auto clone() -> request*;

public js_properties:
    ext::dom_property<ext::string> method;
    ext::dom_property<ext::string> url;
    ext::dom_property<fetch::http::headers*> headers;

    ext::dom_property<ext::string> destination;
    ext::dom_property<ext::string> referrer;
    ext::dom_property<ext::string> referrer_policy;
    ext::dom_property<ext::string> mode;
    ext::dom_property<ext::string> credentials;
    ext::dom_property<ext::string> cache;
    ext::dom_property<ext::string> redirect;
    ext::dom_property<ext::string> integrity;
    ext::dom_property<bool> keep_alive;
    ext::dom_property<bool> is_reload_navigation;
    ext::dom_property<bool> is_history_navigation;
    ext::dom_property<dom::aborting::abort_signal*> signal;
};


struct request_
{
    // url and request type
    ext::string method = "GET";
    url::url& url_record;

    // safety flags
    bool local_urls_only_flag = false;
    bool unsafe_request_flag = false;

    // header management
    ext::vector<std::pair<ext::string, ext::string>> header_list; // TODO : type aliasing
    bool keep_alive;

    // request body and client information
    ext::string body;
    ext::optional<javascript::realms::settings_object_t> client;
    ext::optional<javascript::realms::settings_object_t> reserved_client;
    ext::string replaces_client_id;
    std::variant<ext::string, javascript::realms::settings_object_t> associated_window{ext::string{"client"}};

    // other information
    ext::string service_workers_mode;
    ext::string initiator;
    ext::string destination;
    ext::string priority;
    ext::string origin{"client"};

    std::variant<ext::string, html::internal::policy_container*> {"client"};
};


#endif //SBROWSER_REQUEST_HPP
