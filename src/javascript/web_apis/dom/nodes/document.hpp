#pragma once
#ifndef SBROWSER_DOCUMENT_HPP
#define SBROWSER_DOCUMENT_HPP

#include <variant>

#include <ext/listlike.hpp>
#include <ext/map.hpp>
#include <javascript/interop/attribute_descriptors.hpp>
#include <dom/nodes/node.hpp>

#include <QtWidgets/QScrollArea>

namespace css::cssom_view::other {class caret_position;}
namespace css::css_regions::elements {class named_flow;}

namespace dom {
    namespace helpers {
        struct custom_elements;
        struct node_internals;
    }

    namespace iterators {
        class node_filter;
        class node_iterator;
        class tree_walker;
    }

    namespace nodes {
        class attr;
        class cdata_section;
        class comment;
        class document;
        class document_fragment;
        class document_type;
        class processing_instruction;
        class text;
        class window_proxy;
    }

    namespace ranges {class range;}
    namespace other {class dom_implementation;}
}

namespace html {
    namespace elements {
        class html_body_element;
        class html_head_element;
        class html_html_element;
        class html_image_element;
        class html_form_element;
        class html_link_element;
        class html_script_element;
        class html_title_element;
    }

    namespace internal {
        struct policy_container;
        struct permissions_policy;
        struct module_map;
        struct cross_origin_opener_policy;
        struct document_load_timing_information;
        struct document_unload_timing_information;
        struct browsing_context;
        struct sandboxing_flags;
    }

    namespace other {class location;}
}

namespace svg::elements {class svg_script_element;}


class dom::nodes::document
        : public dom::nodes::node
        , public dom::mixins::non_element_parent_node<document>
        , public dom::mixins::parent_node<document>
        , public dom::mixins::document_or_shadow_root<document>
        , public dom::mixins::document_or_element<document>
        , public ext::listlike<node> {

friends
    friend struct dom::helpers::custom_elements;
    friend struct dom::helpers::node_internals;

public constructors:
    document();
    ~document() override;

private aliases:
    using html_or_svg_script_element = std::variant<html::elements::html_script_element*, svg::elements::svg_script_element*>;

public methods:
    // dom
    new_obj element* create_element(ext::cstring& local_name, ext::cstring_any_map& options = {}) const;
    new_obj element* create_element_ns(ext::cstring& namespace_, ext::cstring& qualified_name, ext::cstring_any_map& options = {}) const;
    new_obj document_fragment* create_document_fragment() const;
    new_obj text* create_text_node(ext::cstring& data) const;
    new_obj cdata_section* create_cdata_section_node(ext::cstring& data) const;
    new_obj comment* create_comment(ext::cstring& data) const;
    new_obj processing_instruction* create_processing_instruction(ext::cstring& target, ext::cstring& data) const;
    new_obj attr* create_attribute(ext::cstring& local_name) const;
    new_obj attr* create_attribute_ns(ext::cstring& namespace_, ext::cstring& qualified_name) const;

    new_obj ranges::range* create_range();
    new_obj iterators::node_iterator* create_node_iterator(node* root, unsigned long what_to_show = 0xFFFFFFFF, iterators::node_filter* filter = nullptr);
    new_obj iterators::tree_walker* create_tree_walker(node* root, unsigned long what_to_show = 0xFFFFFFFF, iterators::node_filter* filter = nullptr);

    same_obj node* import_node(node* node, bool deep = false);
    same_obj node* adopt_node(node* node);

    // html
    ext::vector<node*> get_elements_by_name(ext::cstring& element_name) const;
    document* open() const;
    window_proxy* open(ext::cstring& url, ext::cstring& name, ext::cstring& features) const;
    void close() const;
    template <typename ...strings> void write(strings... text) const;
    template <typename ...strings> void writeln(strings... text) const;
    bool has_focus() const;
    bool exec_command(ext::cstring& command_id, bool show_ui = false, ext::cstring& value = "") const;
    bool query_command_enabled(ext::cstring& command_id) const;
    bool query_command_indeterm(ext::cstring& command_id) const;
    bool query_command_state(ext::cstring& command_id) const;
    bool query_command_supported(ext::cstring& command_id) const;
    ext::string query_command_value(ext::cstring& command_id) const;

    // cssom-view
    element* element_from_point(double x, double y) const;
    ext::vector<element*> elements_from_point(double x, double y) const;
    css::cssom_view::other::caret_position* caret_position_from_point(double x, double y) const;

public properties:
    // dom
    ext::dom_property<ext::string> url;
    ext::dom_property<ext::string> compat_mode;
    ext::dom_property<ext::string> character_set;
    ext::dom_property<ext::string> content_type;
    ext::dom_property<document_type*> doctype;
    ext::dom_property<element*> document_element;
    ext::dom_property<other::dom_implementation*> implementation;

    // html
    ext::dom_property<ext::string> domain;
    ext::dom_property<ext::string> cookie;
    ext::dom_property<ext::string> referrer;
    ext::dom_property<ext::string> last_modified;
    ext::dom_property<ext::string> ready_state;
    ext::dom_property<ext::string> dir;
    ext::dom_property<ext::string> design_mode;
    ext::dom_property<ext::string> title;

    ext::dom_property<html::other::location*> location;
    ext::dom_property<html::elements::html_body_element*> body;
    ext::dom_property<html::elements::html_head_element*> head;

    ext::dom_property<ext::vector<html::elements::html_image_element*>> images;
    ext::dom_property<ext::vector<html::elements::html_link_element*>> links;
    ext::dom_property<ext::vector<html::elements::html_form_element*>> forms;
    ext::dom_property<ext::vector<html::elements::html_script_element*>> scripts;

    ext::dom_property<window_proxy*> default_view;
    ext::dom_property<html_or_svg_script_element> current_script;
    ext::dom_property<ext::string> visibility_state;
    ext::dom_property<bool> hidden;

    // cssom-view
    ext::dom_property<element*> scrolling_element;

    // css-regions
    ext::dom_property<ext::map<ext::string, css::css_regions::elements::named_flow*>*> named_flows;

protected internal_properties:
    // dom
    encoding::encoding* m_encoding;
    ext::string m_type;
    ext::string m_mode;
    ext::string m_origin;

    // html
    html::internal::policy_container* m_policy_container;
    html::internal::permissions_policy* m_permissions_policy;
    html::internal::module_map* m_module_map;
    html::internal::cross_origin_opener_policy* m_cross_origin_opener_policy;
    html::internal::document_load_timing_information* m_load_timing_info;
    html::internal::document_unload_timing_information* m_unload_timing_info;

    bool m_is_initial = false;
    bool m_will_declaratively_refresh = false;
    ext::string m_navigation_id;
    unsigned short m_sandboxing_flag = 0;

    html::internal::browsing_context* m_browsing_context;
    ext::string m_fallback_base_url;
    ext::string m_document_base_url;
    int m_script_blocking_stylesheet_counter;

    int m_throw_on_dynamic_markup_insertion_counter;
    int m_ignore_destructive_writes_counter;

protected internal_methods:
    event_target* get_the_parent(events::event* event) override;

public internal_methods:
    QScrollArea* render() override;

private internal_methods:
    html::elements::html_html_element* get_m_html_element() const;
    html::elements::html_head_element* get_m_head_element() const;
    html::elements::html_title_element* get_m_title_element() const;
    html::elements::html_body_element* get_m_body_element() const;

private accessors:
    // dom
    inline ext::string get_compat_mode() const;
    inline ext::string get_character_set() const;
    inline document_type* get_doctype() const;
    inline element* get_document_element() const;

    // html
    ext::string get_dir() const;
    ext::string get_last_modified() const;
    html::elements::html_body_element* get_body() const;
    html::elements::html_head_element* get_head() const;
    ext::string get_title() const;

    new_obj ext::vector<html::elements::html_image_element*> get_images();
    new_obj ext::vector<html::elements::html_link_element*> get_links();
    new_obj ext::vector<html::elements::html_form_element*> get_forms();
    new_obj ext::vector<html::elements::html_script_element*> get_scripts();

    void set_title(ext::cstring& val);
    void set_body(html::elements::html_body_element* val);
    void set_cookie(ext::cstring& val);

    // cssom-view
    element* get_scrolling_element() const;
};


#endif //SBROWSER_DOCUMENT_HPP
