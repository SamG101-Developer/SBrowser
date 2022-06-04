#pragma once
#ifndef SBROWSER_DOCUMENT_HPP
#define SBROWSER_DOCUMENT_HPP

#include <dom/nodes/node.hpp>
#include <dom/mixins/document_or_element_node.hpp>
#include <dom/mixins/document_or_shadow_root.hpp>
#include <dom/mixins/non_element_parent_node.hpp>
#include <dom/mixins/parent_node.hpp>
#include <dom/xpath/xpath_evaluator.hpp>
namespace dom::nodes {class document;}

#include <variant>
#include <ext/map.hpp>
#include <ext/set.hpp>
#include <ext/listlike.hpp>

#include <javascript/interop/attribute_descriptors.hpp>

#include <performance/time/dom_high_res_timestamp.hpp>

#include <QtWidgets/QScrollArea>

namespace css::cssom_view::other {class caret_position;}
namespace css::css_regions::elements {class named_flow;}

namespace dom::helpers {struct custom_elements;}
namespace dom::helpers {struct node_internals;}
namespace dom::iterators {class node_filter;}
namespace dom::iterators {class node_iterator;}
namespace dom::iterators {class tree_walker;}
namespace dom::nodes {class attr;}
namespace dom::nodes {class cdata_section;}
namespace dom::nodes {class comment;}
namespace dom::nodes {class document_fragment;}
namespace dom::nodes {class document_type;}
namespace dom::nodes {class processing_instruction;}
namespace dom::nodes {class text;}
namespace dom::nodes {class window_proxy;}
namespace dom::ranges {class range;}
namespace dom::other {class dom_implementation;}

namespace encoding {class encoding;}

namespace html::elements {class html_base_element;}
namespace html::elements {class html_body_element;}
namespace html::elements {class html_head_element;}
namespace html::elements {class html_html_element;}
namespace html::elements {class html_image_element;}
namespace html::elements {class html_form_element;}
namespace html::elements {class html_link_element;}
namespace html::elements {class html_script_element;}
namespace html::elements {class html_title_element;}
namespace html::helpers {struct document_internals;}
namespace html::helpers {struct script_internals;}
namespace html::helpers {struct browsing_context_internals;}
namespace html::internal {struct policy_container;}
namespace html::internal {struct permissions_policy;}
namespace html::internal {struct module_map;}
namespace html::internal {struct cross_origin_opener_policy;}
namespace html::internal {struct document_load_timing_information;}
namespace html::internal {struct document_unload_timing_information;}
namespace html::internal {struct browsing_context;}
namespace html::internal {struct sandboxing_flags;}
namespace html::internal {struct session_history_entry;}
namespace html::other {class location;}

namespace svg::elements {class svg_script_element;}

namespace url {class url;}


class dom::nodes::document
        : public node
        , public mixins::non_element_parent_node<document>
        , public mixins::parent_node<document>
        , public mixins::document_or_shadow_root<document>
        , public mixins::document_or_element_node<document>
        , public xpath::xpath_evaluator
        , public ext::listlike<smart_pointer<node>> {

public friends:
    friend struct dom::helpers::custom_elements;
    friend struct dom::helpers::node_internals;
    friend class dom::other::dom_implementation;

    friend class html::elements::html_base_element;
    friend class html::elements::html_image_element;
    friend struct html::helpers::browsing_context_internals;
    friend struct html::helpers::document_internals;
    friend struct html::helpers::script_internals;

public constructors:
    document();

private aliases:
    using html_or_svg_script_element = std::variant<smart_pointer<html::elements::html_script_element>, smart_pointer<svg::elements::svg_script_element>>;

public js_methods:
    // dom
    new_obj [[nodiscard]] auto create_element(const ext::string& local_name, ext::string_any_map_t&& options = {}) const -> element;
    new_obj [[nodiscard]] auto create_element_ns(const ext::string& namespace_, const ext::string& qualified_name, ext::string_any_map_t&& options) const -> element;
    new_obj [[nodiscard]] auto create_document_fragment() const -> document_fragment;
    new_obj [[nodiscard]] auto create_text_node(const ext::string& data) const -> text;
    new_obj [[nodiscard]] auto create_cdata_section_node(const ext::string& data) const -> cdata_section;
    new_obj [[nodiscard]] auto create_comment(const ext::string& data) const -> comment;
    new_obj [[nodiscard]] auto create_processing_instruction(const ext::string& target, const ext::string& data) const -> processing_instruction;
    new_obj [[nodiscard]] auto create_attribute(const ext::string& local_name) const -> attr;
    new_obj [[nodiscard]] auto create_attribute_ns(const ext::string& namespace_, const ext::string& qualified_name) const -> attr;

    new_obj auto create_range() -> ranges::range;
    new_obj auto create_node_iterator(node* root, ulong what_to_show = 0xFFFFFFFF, iterators::node_filter* filter = nullptr) -> iterators::node_iterator;
    new_obj auto create_tree_walker(node* root, ulong what_to_show = 0xFFFFFFFF, iterators::node_filter* filter = nullptr) -> iterators::tree_walker;

    same_obj auto import_node(node* new_node, bool deep = false) -> node*;
    same_obj auto adopt_node(node* new_node) -> node*;

    // html
    auto get_elements_by_name(const ext::string& element_name) const -> ext::vector<node*>;
    auto open() const -> document*;
    auto open(const ext::string& url, const ext::string& name, const ext::string& features) const -> window_proxy*;
    auto close() const -> void;
    template <typename ...strings> auto write(strings... text) const -> void;
    template <typename ...strings> auto writeln(strings... text) const -> void;
    auto has_focus() const -> bool;
    auto exec_command(const ext::string& command_id, bool show_ui = false, const ext::string& value = "") const -> bool;
    auto query_command_enabled(const ext::string& command_id) const -> bool;
    auto query_command_indeterm(const ext::string& command_id) const -> bool;
    auto query_command_state(const ext::string& command_id) const -> bool;
    auto query_command_supported(const ext::string& command_id) const -> bool;
    auto query_command_value(const ext::string& command_id) const -> ext::string;

    // cssom-view
    auto element_from_point(double x, double y) const -> element*;
    auto elements_from_point(double x, double y) const -> ext::vector<element*>;
    auto caret_position_from_point(double x, double y) const -> css::cssom_view::other::caret_position*;

public js_properties:
    // dom
    ext::property<smart_pointer<url::url>> url;
    ext::property<ext::string> compat_mode;
    ext::property<ext::string> character_set;
    ext::property<ext::string> content_type;
    ext::property<document_type*> doctype;
    ext::property<smart_pointer<element>> document_element;
    ext::property<smart_pointer<other::dom_implementation>> implementation;

    // html
    ext::property<ext::string> domain;
    ext::property<ext::string> cookie;
    ext::property<ext::string> referrer;
    ext::property<ext::string> last_modified;
    ext::property<ext::string> ready_state;
    ext::property<ext::string> dir;
    ext::property<ext::string> design_mode;
    ext::property<ext::string> title;

    ext::property<smart_pointer<html::other::location>> location;
    ext::property<smart_pointer<html::elements::html_body_element>> body;
    ext::property<smart_pointer<html::elements::html_head_element>> head;

    ext::property<smart_pointer<ext::vector<smart_pointer<html::elements::html_image_element >>>> images;
    ext::property<smart_pointer<ext::vector<smart_pointer<html::elements::html_link_element  >>>> links;
    ext::property<smart_pointer<ext::vector<smart_pointer<html::elements::html_form_element  >>>> forms;
    ext::property<smart_pointer<ext::vector<smart_pointer<html::elements::html_script_element>>>> scripts;

    ext::property<smart_pointer<window_proxy>> default_view;
    ext::property<html_or_svg_script_element> current_script;
    ext::property<ext::string> visibility_state;
    ext::property<bool> hidden;

    // cssom-view
    ext::property<smart_pointer<element>> scrolling_element;

    // css-regions
    ext::property<ext::map<ext::string, css::css_regions::elements::named_flow*>*> named_flows; // TODO : ptr types, wait to impl spec

public cpp_methods:
    auto qt() const -> QScrollArea* override;
    auto v8(v8::Isolate* isolate) const -> ext::any override;

protected cpp_methods:
    auto get_the_parent(events::event* event) -> event_target* override;

private cpp_methods:
    [[nodiscard]] auto get_m_html_element () const -> smart_pointer<html::elements::html_html_element >;
    [[nodiscard]] auto get_m_head_element () const -> smart_pointer<html::elements::html_head_element >;
    [[nodiscard]] auto get_m_title_element() const -> smart_pointer<html::elements::html_title_element>;
    [[nodiscard]] auto get_m_body_element () const -> smart_pointer<html::elements::html_body_element >;

private cpp_properties:
    // dom
    smart_pointer<encoding::encoding> m_encoding = nullptr;
    ext::string m_type = "xml";
    ext::string m_mode = "no-quirks";
    ext::string m_origin;

    // html
    smart_pointer<html::internal::policy_container> m_policy_container;
    smart_pointer<html::internal::permissions_policy> m_permissions_policy;
    smart_pointer<html::internal::module_map> m_module_map;
    smart_pointer<html::internal::cross_origin_opener_policy> m_cross_origin_opener_policy;
    smart_pointer<html::internal::document_load_timing_information> m_load_timing_info;
    smart_pointer<html::internal::document_unload_timing_information> m_unload_timing_info;

    bool m_is_initial = false;
    bool m_will_declaratively_refresh = false;
    ext::string m_navigation_id = "";

    ext::set<smart_pointer<element>> m_render_blocking_elements {};

    html::internal::browsing_context* m_browsing_context = nullptr;
    ext::string m_fallback_base_url;
    ext::string m_document_base_url;

    int m_script_blocking_stylesheet_counter = 0;

    int m_throw_on_dynamic_markup_insertion_counter = 0;
    int m_ignore_destructive_writes_counter = 0;

    bool m_iframe_load_in_progress_flag = false;
    bool m_mute_iframe_load_flag = false;

    ext::vector<smart_pointer<html::elements::html_script_element>> m_list_of_scripts_that_will_execute_post_parsing;
    ext::vector<smart_pointer<html::elements::html_script_element>> m_list_of_scripts_that_will_execute_soon_as_possible;
    html::elements::html_script_element* m_pending_parsing_blocking_script;

    element* m_focused_area;
    element* m_sequential_focus_navigation_starting_point;
    ext::vector<smart_pointer<element>> m_sequential_focus_navigation_order;

    ext::vector<smart_pointer<element>> m_autofocus_candidates;
    bool m_autofocus_processed_flag;

    bool m_design_mode_enabled = false;

    html::internal::sandboxing_flags& m_active_document_flags_set;
    smart_pointer<html::internal::session_history_entry> m_latest_entry;

    element* m_target_element;

    int m_completely_loaded_time;
    bool m_completely_loaded;

    bool m_salvageable = true;
    bool m_page_showing_flag = false;
    performance::time::dom_high_res_timestamp m_suspension_time;
    ext::vector<performance::time::dom_high_res_timestamp> suspended_timer_handles;
    int m_unload_counter;

private accessors:
    // dom
    [[nodiscard]] auto get_compat_mode() const -> ext::string;
    [[nodiscard]] auto get_character_set() const -> ext::string;
    [[nodiscard]] auto same_obj get_doctype() const -> document_type*;
    [[nodiscard]] auto same_obj get_document_element() const -> element*;

    // html
    [[nodiscard]] auto get_dir() const -> ext::string;
    [[nodiscard]] auto get_design_mode() const -> ext::string;
    [[nodiscard]] auto get_default_view() const -> window_proxy*;
    [[nodiscard]] auto get_last_modified() const -> ext::string;
    [[nodiscard]] auto get_cookie() const -> ext::string;
    [[nodiscard]] auto same_obj get_body() const -> smart_pointer<html::elements::html_body_element>;
    [[nodiscard]] auto same_obj get_head() const -> smart_pointer<html::elements::html_head_element>;
    [[nodiscard]] auto get_title() const -> ext::string;

    [[nodiscard]] auto new_obj get_images () const -> smart_pointer<ext::vector<smart_pointer<html::elements::html_image_element >>>;
    [[nodiscard]] auto new_obj get_links  () const -> smart_pointer<ext::vector<smart_pointer<html::elements::html_link_element  >>>;
    [[nodiscard]] auto new_obj get_forms  () const -> smart_pointer<ext::vector<smart_pointer<html::elements::html_form_element  >>>;
    [[nodiscard]] auto new_obj get_scripts() const -> smart_pointer<ext::vector<smart_pointer<html::elements::html_script_element>>>;

    auto set_design_mode(const ext::string& val) -> void;
    auto set_title(const ext::string& val) -> void;
    auto set_body(html::elements::html_body_element* val) -> void;
    auto set_cookie(const ext::string& val) -> void;
    auto set_ready_state(const ext::string& val) -> void;

    // cssom-view
    [[nodiscard]] auto same_obj get_scrolling_element() const -> element*;
};


#endif //SBROWSER_DOCUMENT_HPP
