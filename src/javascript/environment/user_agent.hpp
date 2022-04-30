#ifndef SBROWSER_USER_AGENT_HPP
#define SBROWSER_USER_AGENT_HPP

namespace javascript::environment {struct user_agent;}

#include <html/helpers/browsing_context_internals.hpp>


struct javascript::environment::user_agent
{
    html::helpers::browsing_context_internals::browsing_context_group_set_t browsing_context_group_set;
};

#endif //SBROWSER_USER_AGENT_HPP
