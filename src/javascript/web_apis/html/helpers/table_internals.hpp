#pragma once
#ifndef SBROWSER_TABLE_INTERNALS_HPP
#define SBROWSER_TABLE_INTERNALS_HPP

#include <ext/string.hpp>

namespace dom::nodes {class document;}
namespace html::helpers {struct table_internals;}


struct html::helpers::table_internals
{
    template <typename T>
    static auto table_create(
            dom::nodes::document* document,
            const ext::string& local_name)
            -> T*
};


#endif //SBROWSER_TABLE_INTERNALS_HPP
