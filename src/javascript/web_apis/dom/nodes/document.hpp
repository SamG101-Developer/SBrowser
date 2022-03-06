#pragma once
#ifndef SBROWSER_DOCUMENT_HPP
#define SBROWSER_DOCUMENT_HPP

#include <variant>

#include <ext/iterables/map.hpp>
#include <ext/mixins/listlike.hpp>
#include <javascript/interop/attribute_descriptors.hpp>

#include <dom/nodes/node.hpp>
#include <dom/mixins/document_or_element_node.hpp>
#include <dom/mixins/document_or_shadow_root.hpp>
#include <dom/mixins/non_element_parent_node.hpp>
#include <dom/mixins/parent_node.hpp>
#include <dom/xpath/xpath_evaluator.hpp>

#include <QtWidgets/QScrollArea>

namespace css::cssom_view::other {class caret_position;}
namespace css::css_regions::elements {class named_flow;}

namespace dom
{
    namespace helpers
    {
        struct custom_elements;
        struct node_internals;
    }

    namespace iterators
    {
        class node_filter;
        class node_iterator;
        class tree_walker;
    }

    namespace nodes
    {
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

namespace encoding {class encoding;}

namespace html
{
    namespace elements
    {
        class html_body_element;
        class html_head_element;
        class html_html_element;
        class html_image_element;
        class html_form_element;
        class html_link_element;
        class html_script_element;
        class html_title_element;
    }

    namespace internal
    {
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


/*
 * https://dom.spec.whatwg.org/#interface-document
 * https://developer.mozilla.org/en-US/docs/Web/API/Document
 *
 * The Document interface represents any web page loaded in the browser and serves as an entry point into the web page's]
 * content, which is the DOM tree.
 *
 * The DOM tree includes elements such as <body> and <table>, among many others. It provides functionality globally to
 * the document, like how to obtain the page's URL and create new elements in the document.
 *
 * The Document interface describes the common properties and methods for any kind of document. Depending on the
 * document's type (e.g. HTML, XML, SVG, …), a larger API is available: HTML documents, served with the "text/html"
 * content type, also implement the HTMLDocument interface, whereas XML and SVG documents implement the XMLDocument
 * interface.
 */
class dom::nodes::document
        : public node
        , public mixins::non_element_parent_node<document>
        , public mixins::parent_node<document>
        , public mixins::document_or_shadow_root<document>
        , public mixins::document_or_element_node<document>
        , public xpath::xpath_evaluator
        , public ext::listlike<node*> {

public friends:
    friend struct dom::helpers::custom_elements;
    friend struct dom::helpers::node_internals;
    friend class dom::other::dom_implementation;

public constructors:
    document();

    ~document() override;

private aliases:
    using html_or_svg_script_element = std::variant<html::elements::html_script_element*, svg::elements::svg_script_element*>;

public methods:
    // dom
    new_obj auto create_element(const ext::string& local_name, const ext::string_any_map& options = {}) const -> element;
    new_obj auto create_element_ns(const ext::string& namespace_, const ext::string& qualified_name, const ext::string_any_map& options = {}) const -> element;
    new_obj auto create_document_fragment() const -> document_fragment;
    new_obj auto create_text_node(const ext::string& data) const -> text;
    new_obj auto create_cdata_section_node(const ext::string& data) const -> cdata_section;
    new_obj auto create_comment(const ext::string& data) const -> comment;
    new_obj auto create_processing_instruction(const ext::string& target, const ext::string& data) const -> processing_instruction;
    new_obj auto create_attribute(const ext::string& local_name) const -> attr;
    new_obj auto create_attribute_ns(const ext::string& namespace_, const ext::string& qualified_name) const -> attr;

    new_obj auto create_range() -> ranges::range;
    new_obj auto create_node_iterator(node* root, unsigned long what_to_show = 0xFFFFFFFF, iterators::node_filter* filter = nullptr) -> iterators::node_iterator;
    new_obj auto create_tree_walker(node* root, unsigned long what_to_show = 0xFFFFFFFF, iterators::node_filter* filter = nullptr) -> iterators::tree_walker;

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

public properties:
    // dom
    ext::dom_property<ext::string, _F> url;
    ext::dom_property<ext::string, _F> compat_mode;
    ext::dom_property<ext::string, _F> character_set;
    ext::dom_property<ext::string, _F> content_type;
    ext::dom_property<document_type*, _F> doctype;
    ext::dom_property<element*, _F> document_element;
    ext::dom_property<other::dom_implementation*, _F> implementation;

    // html
    ext::dom_property<ext::string, _F> domain;
    ext::dom_property<ext::string, _F> cookie;
    ext::dom_property<ext::string, _F> referrer;
    ext::dom_property<ext::string, _F> last_modified;
    ext::dom_property<ext::string, _F> ready_state;
    ext::dom_property<ext::string, _F> dir;
    ext::dom_property<ext::string, _F> design_mode;
    ext::dom_property<ext::string, _F> title;

    ext::dom_property<html::other::location*, _F> location;
    ext::dom_property<html::elements::html_body_element*, _F> body;
    ext::dom_property<html::elements::html_head_element*, _F> head;

    ext::dom_property<ext::vector<html::elements::html_image_element*>, _F> images;
    ext::dom_property<ext::vector<html::elements::html_link_element*>, _F> links;
    ext::dom_property<ext::vector<html::elements::html_form_element*>, _F> forms;
    ext::dom_property<ext::vector<html::elements::html_script_element*>, _F> scripts;

    ext::dom_property<window_proxy*, _F> default_view;
    ext::dom_property<html_or_svg_script_element, _F> current_script;
    ext::dom_property<ext::string, _F> visibility_state;
    ext::dom_property<bool, _F> hidden;

    // cssom-view
    ext::dom_property<element*, _F> scrolling_element;

    // css-regions
    ext::dom_property<ext::map<ext::string, css::css_regions::elements::named_flow*>*, _F> named_flows;

public internal_methods:
    auto render() const -> QScrollArea* override;
    auto v8(v8::Isolate *isolate) const -> ext::any override;

protected internal_methods:
    auto get_the_parent(events::event* event) -> event_target* override;

private internal_methods:
    html::elements::html_html_element* get_m_html_element() const;
    html::elements::html_head_element* get_m_head_element() const;
    html::elements::html_title_element* get_m_title_element() const;
    html::elements::html_body_element* get_m_body_element() const;

private internal_properties:
    // dom
    encoding::encoding* m_encoding = nullptr;
    ext::string m_type = "xml";
    ext::string m_mode = "no-quirks";
    ext::string m_origin;

    // html
    html::internal::policy_container* m_policy_container = nullptr;
    html::internal::permissions_policy* m_permissions_policy = nullptr;
    html::internal::module_map* m_module_map = nullptr;
    html::internal::cross_origin_opener_policy* m_cross_origin_opener_policy = nullptr;
    html::internal::document_load_timing_information* m_load_timing_info = nullptr;
    html::internal::document_unload_timing_information* m_unload_timing_info = nullptr;

    bool m_is_initial = false;
    bool m_will_declaratively_refresh = false;
    ext::string m_navigation_id;
    unsigned short m_sandboxing_flag = 0;

    html::internal::browsing_context* m_browsing_context = nullptr;
    ext::string m_fallback_base_url;
    ext::string m_document_base_url;
    int m_script_blocking_stylesheet_counter = 0;

    int m_throw_on_dynamic_markup_insertion_counter = 0;
    int m_ignore_destructive_writes_counter = 0;

private accessors:
    // dom
    auto get_compat_mode() const -> ext::string;
    auto get_character_set() const -> ext::string;
    auto get_doctype() const -> document_type*;
    auto get_document_element() const -> element*;

    // html
    auto get_dir() const -> ext::string;
    auto get_last_modified() const -> ext::string;
    auto get_body() const -> html::elements::html_body_element*;
    auto get_head() const -> html::elements::html_head_element*;
    auto get_title() const -> ext::string;

    new_obj auto get_images() -> ext::vector<html::elements::html_image_element*>;
    new_obj auto get_links() -> ext::vector<html::elements::html_link_element*>;
    new_obj auto get_forms() -> ext::vector<html::elements::html_form_element*>;
    new_obj auto get_scripts() -> ext::vector<html::elements::html_script_element*>;

    auto set_title(const ext::string& val) -> void;
    auto set_body(html::elements::html_body_element* val) -> void;
    auto set_cookie(const ext::string& val) -> void;
    auto set_ready_state(const ext::string& val) -> void;

    // cssom-view
    auto get_scrolling_element() const -> element*;
};


#endif //SBROWSER_DOCUMENT_HPP
