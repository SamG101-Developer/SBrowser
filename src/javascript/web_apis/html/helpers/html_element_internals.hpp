#pragma once
#ifndef SBROWSER_HTML_ELEMENT_INTERNALS_HPP
#define SBROWSER_HTML_ELEMENT_INTERNALS_HPP

#include <ext/map.hpp>
#include <ext/string.hpp>

namespace dom::nodes {class document;}
namespace dom::nodes {class document_fragment;}
namespace dom::nodes {class node;}
namespace dom::nodes {class text;}
namespace html::elements {class html_details_element;}
namespace html::elements {class html_dlist_element;}
namespace html::elements {class html_element;}
namespace html::elements {class html_olist_element;}
namespace html::elements {class html_li_element;}
namespace html::helpers {struct html_element_internals;}
namespace html::internal {enum class direction_mode;}
namespace html::internal {enum class translation_mode_t;}
namespace html::mixins {template <typename T> class targetable;}

namespace
{
    const ext::string_vector LISTED_ELEMENTS {"button", "output", "select", "textarea"};
    const ext::string_vector SUBMITTABLE_ELEMENTS {"button", "select", "textarea"};
    const ext::string_vector RESETTABLE_ELEMENTS {"output", "select", "textarea"};
    const ext::string_vector AUTOCAPITALIZE_INHERITING_ELEMENTS {"button", "output", "select", "textarea"};
}


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

    static auto rendered_text_fragment(
            const ext::string& input,
            dom::nodes::document* document)
            -> dom::nodes::document_fragment*;

    static auto merge_with_next_text_node(
            dom::nodes::text* text_node)
            -> void;

    static auto walk_subtree_rooted_at_sectioning_element(
            elements::html_element* element)
            -> void;

    static auto starting_value(
            elements::html_olist_element* element)
            -> int;

    static auto list_owner(
            elements::html_li_element* element)
            -> elements::html_element*;

    static auto name_value_group(
            elements::html_dlist_element* element)
            -> ext::vector<ext::string_string_map>;

    static auto process_dt_or_dd(
            dom::nodes::node* node,
            ext::vector<ext::string_string_map>& groups,
            ext::string_string_map& current,
            bool& seen_dd)
            -> void;

    static auto is_listed_element(
            elements::html_element* element)
            -> bool;

    static auto is_submittable_element(
            elements::html_element* element)
            -> bool;

    static auto is_resettable_element(
            elements::html_element* element)
            -> bool;

    static auto is_autocapitalize_inheriting_element(
            elements::html_element* element)
            -> bool;

    static auto details_notification_task_steps(
            elements::html_details_element* details_element)
            -> void;

    static auto ancestor_details_revealing_algorithm(
            elements::html_details_element* details_element)
            -> void;

    static auto is_summary_for_parent_details(
            elements::html_element* summary_element)
            -> bool;

    static auto is_actually_disabled(
            elements::html_element* element)
            -> bool;

    static auto ancestor_hidden_until_found_revealing_algorithm(
            elements::html_element* element)
            -> void;
};


enum class html::internal::direction_mode
{
    LTR,
    RTL,
    AUTO
};


enum class html::internal::translation_mode_t
{
    TRANSLATE_ENABLED,
    NO_TRANSLATE
};


#endif //SBROWSER_HTML_ELEMENT_INTERNALS_HPP
