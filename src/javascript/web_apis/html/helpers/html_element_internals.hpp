#pragma once
#ifndef SBROWSER_HTML_ELEMENT_INTERNALS_HPP
#define SBROWSER_HTML_ELEMENT_INTERNALS_HPP

#include <ext/string.hpp>

namespace dom::nodes {class document;}
namespace dom::nodes {class document_fragment;}
namespace dom::nodes {class text;}
namespace html::elements {class html_element;}
namespace html::helpers {struct html_element_internals;}
namespace html::internal {enum class direction_mode;}
namespace html::internal {enum class translation_mode;}
namespace html::mixins {template <typename T> class targetable;}


struct html::helpers::html_element_internals
{
    static auto directionality(
            elements::html_element* element)
            -> internal::direction_mode;

    static auto being_rendered(
            elements::html_element* element)
            -> bool;

    static auto rendered_text_collection_steps(
            dom::nodes::node* node)
            -> ext::string;

    new_obj static auto rendered_text_fragment(
            const ext::string& input,
            dom::nodes::document* document)
            -> dom::nodes::document_fragment*;

    static auto merge_with_next_text_node(
            dom::nodes::text* text_node)
            -> void;

    template <typename T>
    static auto get_text_elements_target(
            mixins::targetable<T>* element)
            -> ext::string;

    static auto walk_subtree_rooted_at_sectioning_element(
            elements::html_element* element)
            -> void;
};


enum class html::internal::direction_mode
{
    LTR,
    RTL,
    AUTO
};


enum class html::internal::translation_mode
{
    TRANSLATE_ENABLED,
    NO_TRANSLATE
};


#endif //SBROWSER_HTML_ELEMENT_INTERNALS_HPP
