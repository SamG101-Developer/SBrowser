#include "document.hpp"

#include <ctime>
#include <format>

#include <javascript/environment/realms.hpp>

#include <dom/events/event.hpp>

#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/event_dispatching.hpp>
#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/node_internals.hpp>
#include <dom/helpers/namespaces.hpp>
#include <dom/helpers/shadows.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/cdata_section.hpp>
#include <dom/nodes/character_data.hpp>
#include <dom/nodes/comment.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/processing_instruction.hpp>
#include <dom/nodes/text.hpp>

#include <dom/other/dom_implementation.hpp>

#include <dom/ranges/range.hpp>

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>


dom::nodes::document::document()
        : node()
        , mixins::non_element_parent_node<document>()
        , mixins::parent_node<document>()
        , mixins::document_or_shadow_root<document>()
        , mixins::document_or_element_node<document>()
        , xpath::xpath_evaluator()
        , ext::listlike<node*>() {

    compat_mode.get = [this] {return get_compat_mode();};
    character_set.get = [this] {return get_character_set();};
    doctype.get = [this] {return get_doctype();};
    document_element.get = [this] {return get_document_element();};

    dir.get = [this] {return get_compat_mode();};
    last_modified.get = [this] {return get_last_modified();};
    body.get = [this] {return get_body();};
    head.get = [this] {return get_head();};
    title.get = [this] {return get_title();};
    images.get = [this] {return get_images();};
    links.get = [this] {return get_links();};
    forms.get = [this] {return get_forms();};
    scripts.get = [this] {return get_scripts();};

    title.set = [this](auto&& PH1) {set_title(std::forward<decltype(PH1)>(PH1));};
    body.set = [this](auto&& PH1) {set_body(std::forward<decltype(PH1)>(PH1));};
    cookie.set = [this](auto&& PH1) {set_cookie(std::forward<decltype(PH1)>(PH1));};
    ready_state.set = [this](auto&& PH1) {set_ready_state(std::forward<decltype(PH1)>(PH1));};

    node_type = DOCUMENT_NODE;
    node_name = "#document";
    url = "about:blank";
    content_type = "application/xml";
    implementation = new other::dom_implementation{};
    ready_state = "complete";

    m_type = "xml";
    m_mode = "no-quirks";
    m_origin = javascript::realms::surrounding_agent().get<ext::cstring&>("origin");

    m_rendered_widget = new QScrollArea{nullptr};
    render()->setWidget(new QWidget{render()});
    render()->setLayout(new QVBoxLayout{render()->widget()});
    render()->setWidgetResizable(true);
    render()->show();
}


dom::nodes::element*
dom::nodes::document::create_element(
        ext::cstring& local_name,
        ext::cstring_any_map& options) const {

    auto is = ext::any_cast<bool>(options.at("is"));

    ext::string html_qualified_name = m_type == "html"
            ? local_name.new_lowercase()
            : local_name;

    ext::string namespace_ = m_type == "html" or content_type == "application/xhtml+xml"
            ? helpers::namespaces::HTML
            : helpers::namespaces::NONE;

    return helpers::custom_elements::create_an_element(this, local_name, namespace_, "", is, true);
}


dom::nodes::element*
dom::nodes::document::create_element_ns(
        ext::cstring& namespace_,
        ext::cstring& qualified_name,
        ext::cstring_any_map& options) const {

    auto is = ext::any_cast<bool>(options.at("is"));
    auto [html_qualified_namespace, prefix, local_name] = helpers::namespaces::validate_and_extract(namespace_, qualified_name);
    return helpers::custom_elements::create_an_element(this, local_name, html_qualified_namespace, prefix, is, true);
}


dom::nodes::document_fragment*
dom::nodes::document::create_document_fragment() const {

    auto* document_fragment_node = new document_fragment{};
    document_fragment_node->owner_document = this;
    return document_fragment_node;
}


dom::nodes::text*
dom::nodes::document::create_text_node(ext::cstring& data) const {

    auto* text_node = new text{};
    text_node->data = data;
    text_node->owner_document = this;
    return text_node;
}


dom::nodes::cdata_section*
dom::nodes::document::create_cdata_section_node(ext::cstring& data) const {

    helpers::exceptions::throw_v8_exception(
            "html documents cannot create cdata_section nodes",
            NOT_SUPPORTED_ERR,
            [this] {return m_type == "html";});

    helpers::exceptions::throw_v8_exception(
            "cdata_section data cannot contain ']]>'",
            INVALID_CHARACTER_ERR,
            [data] {return data.contains("]]>");});

    auto* cdata_section_node = new cdata_section{};
    cdata_section_node->data = data;
    cdata_section_node->owner_document = this;
    return cdata_section_node;
}


dom::nodes::comment*
dom::nodes::document::create_comment(ext::cstring& data) const {

    auto* comment_node = new dom::nodes::comment{};
    comment_node->data = data;
    comment_node->owner_document = this;
    return comment_node;
}


dom::nodes::processing_instruction*
dom::nodes::document::create_processing_instruction(
        ext::cstring& target,
        ext::cstring& data) const {

    helpers::exceptions::throw_v8_exception(
            "processing_instruction data cannot contain '?>'",
            INVALID_CHARACTER_ERR,
            [data] {return data.contains("?>");});

    auto* processing_instruction_node = new processing_instruction{};
    processing_instruction_node->target = target;
    processing_instruction_node->data = data;
    processing_instruction_node->owner_document = this;
    return processing_instruction_node;
}


dom::nodes::attr*
dom::nodes::document::create_attribute(ext::cstring& local_name) const {

    ext::string html_qualified_namespace = m_type == "html"
            ? local_name.new_lowercase()
            : local_name;

    dom::nodes::attr* attribute;
    attribute->local_name = local_name;
    attribute->owner_document = this;
    return attribute;
}


dom::nodes::attr*
dom::nodes::document::create_attribute_ns(
        ext::cstring& namespace_,
        ext::cstring& qualified_name) const {

    auto [html_qualified_namespace, prefix, local_name] = helpers::namespaces::validate_and_extract(namespace_, qualified_name);

    auto* attribute = new attr{};
    attribute->namespace_uri = html_qualified_namespace;
    attribute->prefix = prefix;
    attribute->local_name = local_name;
    attribute->owner_document = this;
    return attribute;
}


dom::ranges::range*
dom::nodes::document::create_range() {

    auto* range = new ranges::range{};
    range->start_container = this;
    range->end_container = this;
    range->start_offset = 0;
    range->end_offset = 0;
    return range;
}


dom::iterators::node_iterator*
dom::nodes::document::create_node_iterator(
        node* root,
        unsigned long what_to_show,
        iterators::node_filter* filter) {

    auto* iterator = new iterators::node_iterator{};
    iterator->root = root;
    iterator->reference_node = root;
    iterator->pointer_before_reference_node = root;
    iterator->what_to_show = what_to_show;
    iterator->filter = filter;
    return iterator;
}


dom::iterators::tree_walker*
dom::nodes::document::create_tree_walker(
        node* root,
        unsigned long what_to_show,
        iterators::node_filter* filter) {

    auto* walker = new iterators::tree_walker{};
    walker->root = root;
    walker->current_node = this;
    walker->what_to_show = what_to_show;
    walker->filter = filter;
    return walker;
}


dom::nodes::node*
dom::nodes::document::import_node(
        node* node,
        bool deep) {

    helpers::exceptions::throw_v8_exception(
            "cannot import a document node",
            NOT_SUPPORTED_ERR,
            [node] {return dynamic_cast<document*>(node);});

    helpers::exceptions::throw_v8_exception(
            "cannot import a shadowroot node",
            NOT_SUPPORTED_ERR,
            [node] {return helpers::shadows::is_shadow_root(node);});

    return helpers::node_internals::clone(node, this, true);
}


dom::nodes::node*
dom::nodes::document::adopt_node(node* node) {

    helpers::exceptions::throw_v8_exception(
            "cannot adopt a document node",
            NOT_SUPPORTED_ERR,
            [node] {return dynamic_cast<document*>(node);});

    helpers::exceptions::throw_v8_exception(
            "cannot adopt a shadowroot node",
            HIERARCHY_REQUEST_ERR,
            [node] {return helpers::shadows::is_shadow_root(node);});
}


ext::vector<dom::nodes::node*>
dom::nodes::document::get_elements_by_name(ext::cstring& element_name) const {

    return helpers::trees::descendants(this)
            .cast_all<element*>()
            .filter([element_name](element* node) {return node->get_m_qualified_name() == element_name;})
            .cast_all<node*>();
}


dom::nodes::document*
dom::nodes::document::open() const {

    return html::helpers::elements::document_open_steps(this);
}


dom::nodes::window_proxy*
dom::nodes::document::open(
        ext::cstring& url,
        ext::cstring& name,
        ext::cstring& features) const {

    helpers::exceptions::throw_v8_exception(
            "cannot open a non-active document",
            INVALID_ACCESS_ERR,
            [this] {return not helpers::node_internals::is_document_fully_active(this);});

    return html::helpers::elements::window_open_steps(this);
}


void
dom::nodes::document::close() const {

    helpers::exceptions::throw_v8_exception(
            "cannot close an xml document",
            INVALID_STATE_ERR,
            [this] {return m_type == "xml";});

    helpers::exceptions::throw_v8_exception(
            "cannot close a document whose dynamic-markup-counter > 0",
            INVALID_STATE_ERR,
            [this] {return m_throw_on_dynamic_markup_insertion_counter > 0;});

    // TODO
}


template <typename ...strings>
void
dom::nodes::document::write(strings... text) const {
    html::helpers::elements::document_write_steps(this, ext::concatenate_strings(text...))
}


template <typename ...strings>
void
dom::nodes::document::writeln(strings... text) const {
    ext::vector<ext::string> new_lined_text;
    new_lined_text = {text...};
    new_lined_text.template for_each([](ext::string& string) -> ext::string {string += "\n";});
    write(new_lined_text);
}


bool dom::nodes::document::has_focus() const {

    return html::helpers::elements::has_focus_steps(this);
}


dom::nodes::element*
dom::nodes::document::element_from_point(
        double x,
        double y) const {

    return elements_from_point(x, y).front(); // TODO at first found doesn't happen currently
}


ext::vector<dom::nodes::element*>
dom::nodes::document::elements_from_point(
        double x,
        double y) const {

    if (x < 0 or y < 0)
        return ext::vector<element*>{nullptr};

    return helpers::trees::descendants(this)
            .cast_all<element*>()
            .filter([x, y](element* node) {node->render()->geometry().contains(x, y);});
}


css::cssom_view::other::caret_position*
dom::nodes::document::caret_position_from_point(
        double x,
        double y) const {

    if (not render()->widget()) return nullptr;
    if (x < 0 or y < 0) return nullptr;

    auto* caret_position = nullptr;
    if (auto* text_insertion_widget = dynamic_cast<QLineEdit*>(render()->widget()->childAt(x, y))) {
        caret_position = new css::cssom_view::other::caret_position{};
        caret_position->m_range = new ranges::raneg{};
        caret_position->offset_node = caret_position->m_range->start_container;
        caret_position->offset = caret_position->m_range->start_offset;
    }

    return caret_position;
}


dom::nodes::event_target*
dom::nodes::document::get_the_parent(events::event* event) {

    return event->type == "load" or not m_browsing_context ? nullptr : &javascript::realms::relevant_global_object();
}

html::elements::html_html_element* dom::nodes::document::get_m_html_element() const {
    return ext::property_dynamic_cast<html::elements::html_html_element*>(document_element);
}

html::elements::html_head_element* dom::nodes::document::get_m_head_element() const {
    return get_m_html_element()->children->cast_all<html::elements::html_head_element*>().front();
}

html::elements::html_title_element* dom::nodes::document::get_m_title_element() const {
    return helpers::trees::descendants(this).cast_all<html::elements::html_title_element*>().front();
}

html::elements::html_body_element* dom::nodes::document::get_m_body_element() const {
    return children->cast_all<html::elements::html_body_element*>().front();
}

ext::string dom::nodes::document::get_compat_mode() const {
    return m_mode == "quirks" ? "BackCompat" : "CSS1Compat";
}

ext::string dom::nodes::document::get_character_set() const {
    return m_encoding->name;
}

dom::nodes::document_type* dom::nodes::document::get_doctype() const {
    return child_nodes->cast_all<document_type*>().front();
}

dom::nodes::element* dom::nodes::document::get_document_element() const {
    return child_nodes->cast_all<element*>().front();
}

ext::string dom::nodes::document::get_dir() const {
    return get_m_html_element()->dir;
}

ext::string dom::nodes::document::get_last_modified() const {
    return "" /* TODO from header */;
}

html::elements::html_body_element* dom::nodes::document::get_body() const {
    return get_m_body_element();
}

html::elements::html_head_element* dom::nodes::document::get_head() const {
    return get_m_head_element();
}

ext::string dom::nodes::document::get_title() const {
    return helpers::trees::child_text_content(get_m_title_element());
}

ext::vector<html::elements::html_image_element*> dom::nodes::document::get_images() {
    return helpers::trees::descendants(this).cast_all<html::elements::html_image_element*>();
}

ext::vector<html::elements::html_link_element*> dom::nodes::document::get_links() {
    return helpers::trees::descendants(this).cast_all<html::elements::html_link_element*>();
}

ext::vector<html::elements::html_form_element*> dom::nodes::document::get_forms() {
    return helpers::trees::descendants(this).cast_all<html::elements::html_form_element*>();
}

ext::vector<html::elements::html_script_element*> dom::nodes::document::get_scripts() {
    return helpers::trees::descendants(this).cast_all<html::elements::html_script_element*>().filter([](auto* element) {return element->href;});
}

void
dom::nodes::document::set_title(ext::cstring& val) {

    element* title_element;
    if (dynamic_cast<svg::nodes::svg_element*>(document_element)) {
        title_element = document_element->child_nodes->cast_all<svg::nodes::svg_title_element*>().front();

        if (not title_element) {
            title_element = helpers::custom_elements::create_an_element(document_element->owner_document, "title", helpers::namespaces::SVG);
            helpers::mutation_algorithms::insert(title_element, document_element, document_element->child_nodes->front());
        }
        helpers::node_internals::string_replace_all(val, title_element);
    }

    else if (document_element->namespace_uri == helpers::namespaces::HTML) {
        if (not get_m_title_element() and not get_m_head_element()) return;
        if (get_m_title_element()) title_element = get_m_title_element();

        if (not title_element) {
            title_element = helpers::custom_elements::create_an_element(document_element->owner_document, "title", helpers::namespaces::HTML);
            helpers::mutation_algorithms::append(title_element, get_m_head_element());
        }
        helpers::node_internals::string_replace_all(val, title_element);
    }
}


void
dom::nodes::document::set_body(html::elements::html_body_element* val) {

    helpers::exceptions::throw_v8_exception(
            "body attribute must be a HTMLBodyElement",
            HIERARCHY_REQUEST_ERR,
            [val] {return dynamic_cast<html::elements::html_body_element*>(val);});

    helpers::exceptions::throw_v8_exception(
            "settings a null body attribute requires a document element to be present",
            HIERARCHY_REQUEST_ERR,
            [val, this] {return not val and not document_element;});

    helpers::mutation_algorithms::append(val, document_element);
}


void
dom::nodes::document::set_cookie(ext::cstring& val) {

    if (html::helpers::cookies::is_cookie_averse_document(this)) return;

    helpers::exceptions::throw_v8_exception(
            "cannot set the cookie of a document that has an opaque origin",
            SECURITY_ERR,
            [this] {return m_origin == "opaque";});

    cookie << val;
}


void dom::nodes::document::set_ready_state(ext::cstring& val) {
    if (ready_state == val) return;
    // TODO : parser stuff
    helpers::event_dispatching::fire_event<>("readyStateChange", this);
}


dom::nodes::element* dom::nodes::document::get_scrolling_element() const {
    return m_mode == "quirks" and body
            ? ext::property_dynamic_cast<element*>(body)
            : dynamic_cast<element*>(helpers::trees::root(this));
}
