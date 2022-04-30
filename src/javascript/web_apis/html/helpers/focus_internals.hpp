#pragma once
#ifndef SBROWSER_FOCUS_INTERNALS_HPP
#define SBROWSER_FOCUS_INTERNALS_HPP

namespace html::helpers {struct focus_internals;}

#include <variant>
#include <ext/string.hpp>
#include <ext/type_traits.hpp>

namespace dom::nodes {class event_target;}
namespace dom::nodes {class document;}
namespace dom::nodes {class node;}

namespace html::elements {class html_dialog_element;}
namespace html::elements {class html_element;}
namespace html::internal {struct browsing_context;}


struct html::helpers::focus_internals
{ // TODO : lots or return types should be dom::nodes::node* (document or element can be focusable?)

    using focus_target_t = std::variant<elements::html_element*, internal::browsing_context*>;

    static auto currently_focussed_area_of_top_level_browsing_context(
            internal::browsing_context* top_level_browsing_context)
            -> dom::nodes::node*;

    static auto current_focus_chain_of_top_level_browsing_context(
            internal::browsing_context* top_level_browsing_context)
            -> ext::vector<elements::html_element*>;

    static auto focus_chain(
            internal::browsing_context* top_level_browsing_context)
            -> ext::vector<elements::html_element*>;

    static auto is_focusable(
            elements::html_element* element)
            -> bool;

    static auto is_sequentially_focusable(
            elements::html_element* element)
            -> bool;

    static auto is_click_focusable(
            elements::html_element* element)
            -> bool;

    static auto is_focus_navigation_scope(
            elements::html_element* element)
            -> bool;

    static auto focus_navigation_scope(
            elements::html_element* element)
            -> ext::vector<elements::html_element*>;

    static auto associated_focus_navigation_owner(
            elements::html_element* element)
            -> elements::html_element*;

    static auto tabindex_ordered_focus_navigation_scope(
            elements::html_element* element)
            -> ext::vector<elements::html_element*>;

    static auto flattened_tabindex_ordered_focus_navigation_scope(
            elements::html_element* element)
            -> ext::vector<elements::html_element*>;

    static auto get_the_focusable_area(
            std::variant<elements::html_element*, internal::browsing_context*> focus_target,
            const ext::string& focus_trigger = "default")
            -> elements::html_element*;

    static auto focus_delegate(
            focus_target_t focus_target,
            const ext::string& focus_trigger = "other")
            -> elements::html_element*;

    static auto autofocus_delegate(
            focus_target_t focus_target,
            const ext::string& focus_trigger)
            -> elements::html_element*;

    static auto focusing_steps(
            focus_target_t focus_target,
            std::optional<focus_target_t> fallback_target = null,
            std::optional<ext::string> focus_trigger      = null)
            -> void;

    static auto unfocusing_steps(
            focus_target_t old_focus_target)
            -> void;

    static auto focus_update_steps(
            ext::vector<dom::nodes::node*> old_chain,
            ext::vector<dom::nodes::node*> new_chain,
            focus_target_t new_focus_target)
            -> void;

    static auto has_focus_steps(
            dom::nodes::document* target)
            -> bool;

    static auto fire_focus_event(
            const ext::string& e,
            dom::nodes::event_target* t,
            dom::nodes::event_target* r)
            -> void;

    static auto sequential_navigation_search_algorithm(
            focus_target_t starting_point,
            const ext::string& direction,
            const ext::string& selection_algorithm)
            -> elements::html_element*;

    static auto nearest_ancestor_autofocus_scoping_root_element(
            elements::html_element* element)
            -> elements::html_dialog_element*;

    static auto flush_autfocus_candidates(
            dom::nodes::document* top_document)
            -> void;
};


#endif //SBROWSER_FOCUS_INTERNALS_HPP
