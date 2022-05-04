#pragma once
#ifndef SBROWSER_HISTORY_INTERNALS_HPP
#define SBROWSER_HISTORY_INTERNALS_HPP

namespace html::helpers {struct history_internals;}

#include <ext/any.hpp>
#include <ext/string.hpp>

namespace dom::nodes {class document;}
namespace html::internal {struct browsing_context;}
namespace html::other {struct history;}
namespace url {class url;}


struct html::helpers::history_internals
{
    static auto traverse_history_by_delta(
            long delta,
            internal::browsing_context* context)
            -> void;

    static auto url_and_history_update_steps(
            dom::nodes::document* document,
            url::url& new_url,
            const ext::string& serialized_data,
            bool is_push)
            -> void;

    static auto shared_history_push_replace_state_steps(
            other::history* history,
            const ext::any& value,
            const ext::string& url,
            bool is_push)
            -> void;

    static auto rewrite_url(
            dom::nodes::document* document,
            url::url& target_url)
            -> void;
};


#endif //SBROWSER_HISTORY_INTERNALS_HPP
