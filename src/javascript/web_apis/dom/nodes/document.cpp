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

#include <dom/iterators/node_iterator.hpp>
#include <dom/iterators/tree_walker.hpp>

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
#include <QtCore/QPointer>


/*
 * https://dom.spec.whatwg.org/#dom-document-document
 * https://developer.mozilla.org/en-US/docs/Web/API/Document/Document
 *
 * The Document constructor creates a new Document object that is a web page loaded in the browser and serving as an
 * entry point into the page's content.
 */
dom::nodes::document::document()
        : node()
        , mixins::non_element_parent_node<document>()
        , mixins::parent_node<document>()
        , mixins::document_or_shadow_root<document>()
        , mixins::document_or_element_node<document>()
        , xpath::xpath_evaluator()
        , ext::listlike<node*>()
{
    // set the custom accessors
    compat_mode.getter      = [this] {return get_compat_mode();};
    character_set.getter    = [this] {return get_character_set();};
    doctype.getter          = [this] {return get_doctype();};
    document_element.getter = [this] {return get_document_element();};

    dir.getter           = [this] {return get_compat_mode();};
    last_modified.getter = [this] {return get_last_modified();};
    body.getter          = [this] {return get_body();};
    head.getter          = [this] {return get_head();};
    title.getter         = [this] {return get_title();};
    images.getter        = [this] {return get_images();};
    links.getter         = [this] {return get_links();};
    forms.getter         = [this] {return get_forms();};
    scripts.getter       = [this] {return get_scripts();};

    title.setter       = [this](auto&& PH1) {set_title(std::forward<decltype(PH1)>(PH1));};
    body.setter        = [this](auto&& PH1) {set_body(std::forward<decltype(PH1)>(PH1));};
    cookie.setter      = [this](auto&& PH1) {set_cookie(std::forward<decltype(PH1)>(PH1));};
    ready_state.setter = [this](auto&& PH1) {set_ready_state(std::forward<decltype(PH1)>(PH1));};

    // set the properties
    node_type      << DOCUMENT_NODE;
    node_name      << "#document";
    url            << "about:blank";
    content_type   << "application/xml";
    implementation << new other::dom_implementation{};
    ready_state    << "complete";

    // set the attributes
    m_origin = javascript::realms::surrounding_agent().get<const ext::string&>("origin");

    // create the widget representation
    auto rendered_widget = QPointer<QScrollArea>{};
    rendered_widget->setWidget(QPointer<QWidget>{});
    rendered_widget->widget()->setLayout(QPointer<QVBoxLayout>{});
    rendered_widget->setWidgetResizable(true);
    rendered_widget->show();
    m_rendered_widget = rendered_widget;
}


auto dom::nodes::document::create_element(
        const ext::string& local_name,
        const ext::string_any_map& options) const
        -> dom::nodes::element
{
    // get the <is> option as a string
    auto is = options.at("is").to<ext::string>();

    // if the document type is html then set the html qualified name to lowercase
    const ext::string html_qualified_name = m_type == "html"
            ? local_name.new_lowercase()
            : local_name;

    // if the document type or content type is html then set the namespace to HTML, otherwise NONE
    const ext::string namespace_ = m_type == "html" or content_type == ext::string{"application/xhtml+xml"}
            ? helpers::namespaces::HTML
            : helpers::namespaces::NONE;

    // return the created element given the set of options
    return *helpers::custom_elements::create_an_element(const_cast<document*>(this), local_name, namespace_, "", is, true);
}


auto dom::nodes::document::create_element_ns(
        const ext::string& namespace_,
        const ext::string& qualified_name,
        const ext::string_any_map& options) const
        -> dom::nodes::element
{
    // get the <is> option as a string
    auto is = options.at("is").to<ext::string>();

    // extract the namespace, prefix and local name from the namespace and qualified name
    auto [html_qualified_namespace, prefix, local_name] = helpers::namespaces::validate_and_extract(namespace_, qualified_name);

    // return the created element given the set of options
    return *helpers::custom_elements::create_an_element(const_cast<document*>(this), local_name, html_qualified_namespace, prefix, is, true);
}


auto dom::nodes::document::create_document_fragment() const -> dom::nodes::document_fragment
{
    // create a new document fragment, and set the owner document to this document
    document_fragment document_fragment_node{};
    document_fragment_node.owner_document = const_cast<document*>(this);

    // return the document fragment
    return document_fragment_node;
}


auto dom::nodes::document::create_text_node(const ext::string& data) const -> dom::nodes::text
{
    // create a new text node, set the text to data, and set the owner document to this document
    text text_node{};
    text_node.data = data;
    text_node.owner_document = const_cast<document*>(this);

    // return the text node
    return text_node;
}


auto dom::nodes::document::create_cdata_section_node(const ext::string& data) const -> dom::nodes::cdata_section
{
    // if the document type is html, then throw a not supported error
    helpers::exceptions::throw_v8_exception(
            "html documents cannot create cdata_section nodes",
            NOT_SUPPORTED_ERR,
            [this] {return m_type == "html";});

    // if ']]>' is in the data, then throw an invalid character error
    helpers::exceptions::throw_v8_exception(
            "cdata_section data cannot contain ']]>'",
            INVALID_CHARACTER_ERR,
            [data] {return data.contains("]]>");});

    // create a new cdata_section node, and set the owner document to this document
    cdata_section cdata_section_node{};
    cdata_section_node.data = data;
    cdata_section_node.owner_document = const_cast<document*>(this);

    // return the cdata_section node
    return cdata_section_node;
}


auto dom::nodes::document::create_comment(const ext::string& data) const -> dom::nodes::comment
{
    // create a new comment node, and set the owner document to this document
    comment comment_node{};
    comment_node.data = data;
    comment_node.owner_document = const_cast<document*>(this);

    // return the comment node
    return comment_node;
}


auto dom::nodes::document::create_processing_instruction(
        const ext::string& target,
        const ext::string& data) const
        -> dom::nodes::processing_instruction
{
    // if '?>' is in the data, then throw an invalid character error
    helpers::exceptions::throw_v8_exception(
            "processing_instruction data cannot contain '?>'",
            INVALID_CHARACTER_ERR,
            [data] {return data.contains("?>");});

    // create a new comment node, and set the owner document to this document
    processing_instruction processing_instruction_node{};
    processing_instruction_node.target = target;
    processing_instruction_node.data = data;
    processing_instruction_node.owner_document = const_cast<document*>(this);

    // return the processing instruction
    return processing_instruction_node;
}


auto dom::nodes::document::create_attribute(const ext::string& local_name) const -> dom::nodes::attr
{
    // if the document type is html then set the local name to lowercase
    const ext::string html_qualified_namespace = m_type == "html"
            ? local_name.new_lowercase()
            : local_name;

    // create a new attribute node, and set the owner document to this document
    attr attribute{};
    attribute.local_name = local_name;
    attribute.owner_document = const_cast<document*>(this);

    // return the attribute node
    return attribute;
}


auto dom::nodes::document::create_attribute_ns(
        const ext::string& namespace_,
        const ext::string& qualified_name) const
        -> dom::nodes::attr
{
    // extract the namespace, prefix and local name from the namespace and qualified name
    auto [html_qualified_namespace, prefix, local_name] = helpers::namespaces::validate_and_extract(namespace_, qualified_name);

    // create a new attribute node, and set the owner document to this document
    attr attribute{};
    attribute.namespace_uri = html_qualified_namespace;
    attribute.prefix = prefix;
    attribute.local_name = local_name;
    attribute.owner_document = const_cast<document*>(this);

    // return teh attribute node
    return attribute;
}


auto dom::nodes::document::create_range()
        -> dom::ranges::range
{
    // create a new range, and set the starting and ending nodes to this node, with the offsets at 0
    ranges::range range{};
    range.start_container = this;
    range.end_container = this;
    range.start_offset = 0;
    range.end_offset = 0;

    // return the range
    return range;
}


auto dom::nodes::document::create_node_iterator(
        node* const root,
        const unsigned long what_to_show,
        iterators::node_filter* const filter)
        -> dom::iterators::node_iterator
{
    // create a new node iterator
    iterators::node_iterator iterator{};
    iterator.root = root;
    iterator.reference_node = root;
    iterator.pointer_before_reference_node = root;
    iterator.what_to_show = what_to_show;
    iterator.filter = filter;

    // return the node iterator
    return iterator;
}


auto dom::nodes::document::create_tree_walker(
        node* const root,
        const unsigned long what_to_show,
        iterators::node_filter* const filter)
        -> dom::iterators::tree_walker
{
    // create a new tree walker
    iterators::tree_walker walker{};
    walker.root = root;
    walker.current_node = this;
    walker.what_to_show = what_to_show;
    walker.filter = filter;

    // return the tree walker
    return walker;
}


auto dom::nodes::document::import_node(
        node* const node,
        const bool deep)
        -> dom::nodes::node*
{
    // if the node being imported is a document, then throw a not supported error
    helpers::exceptions::throw_v8_exception(
            "cannot import a document node",
            NOT_SUPPORTED_ERR,
            [node] {return dynamic_cast<document*>(node);});

    // if the node being imported is a shadow_root, then throw a not supported error
    helpers::exceptions::throw_v8_exception(
            "cannot import a shadowroot node",
            NOT_SUPPORTED_ERR,
            [node] {return helpers::shadows::is_shadow_root(node);});

    // clone the node into this document
    return helpers::node_internals::clone(node, this, deep);
}


auto dom::nodes::document::adopt_node(node* const node) -> dom::nodes::node*
{
    // if the node being adopted is a document, then throw a not supported error
    helpers::exceptions::throw_v8_exception(
            "cannot adopt a document node",
            NOT_SUPPORTED_ERR,
            [node] {return dynamic_cast<document*>(node);});

    // if the node being adopted is a shadow_root, then throw a hierarchy request error
    helpers::exceptions::throw_v8_exception(
            "cannot adopt a shadowroot node",
            HIERARCHY_REQUEST_ERR,
            [node] {return helpers::shadows::is_shadow_root(node);});

    // return nullptr if the node being imported is a document fragment that has a host
    if (dynamic_cast<document_fragment*>(node)->host)
        return nullptr;

    // return the node after running the adopt helper method
    helpers::node_internals::adopt(node, this);
    return node;
}


auto dom::nodes::document::get_elements_by_name(const ext::string& element_name) const -> ext::vector<dom::nodes::node*>
{
    // filter the element descendants, by matching the elements with the same qualified name as element name, and
    // convert them back into nodes before returning the list
    return helpers::trees::descendants(this)
            .cast_all<element*>()
            .filter([element_name](const element* const node) {return node->get_m_qualified_name() == element_name;})
            .cast_all<node*>();
}


auto dom::nodes::document::open() const -> dom::nodes::document*
{
    // TODO
    return html::helpers::elements::document_open_steps(this);
}


auto dom::nodes::document::open(
        const ext::string& url,
        const ext::string& name,
        const ext::string& features) const
        -> dom::nodes::window_proxy*
{
    // TODO
    helpers::exceptions::throw_v8_exception(
            "cannot open a non-active document",
            INVALID_ACCESS_ERR,
            [this] {return not helpers::node_internals::is_document_fully_active(const_cast<document*>(this));});

    return html::helpers::elements::window_open_steps(this);
}


auto dom::nodes::document::close() const -> void
{
    // TODO
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
auto dom::nodes::document::write(strings... text) const -> void
{
    // TODO
    html::helpers::elements::document_write_steps(this, ext::concatenate_strings(text...))
}


template <typename ...strings>
auto dom::nodes::document::writeln(strings... text) const -> void
{
    // TODO
    ext::vector<ext::string> new_lined_text;
    new_lined_text = {text...};
    new_lined_text.template for_each([](ext::string& string) -> ext::string {string += "\n";});
    write(new_lined_text);
}


auto dom::nodes::document::has_focus() const -> bool
{
    //TODO
    return html::helpers::elements::has_focus_steps(this);
}


auto dom::nodes::document::element_from_point(
        const double x,
        const double y) const
        -> dom::nodes::element*
{
    // TODO
    return elements_from_point(x, y).front(); // TODO at first found doesn't happen currently
}


auto dom::nodes::document::elements_from_point(
        const double x,
        const double y) const
        -> ext::vector<dom::nodes::element*>
{
    if (x < 0 or y < 0)
        return ext::vector<element*>{nullptr};

    return helpers::trees::descendants(this)
            .cast_all<element*>()
            .filter([x, y](element* node) {node->render()->geometry().contains(x, y);});
}


auto dom::nodes::document::caret_position_from_point(
        const double x,
        const double y) const
        -> css::cssom_view::other::caret_position*
{

    if (not render()->widget())
        return nullptr;
    
    if (x < 0 or y < 0)
        return nullptr;

    auto* caret_position = nullptr;
    if (auto* text_insertion_widget = dynamic_cast<QLineEdit*>(render()->widget()->childAt(x, y))) {
        caret_position = new css::cssom_view::other::caret_position{};
        caret_position->m_range = new ranges::raneg{};
        caret_position->offset_node = caret_position->m_range->start_container;
        caret_position->offset = caret_position->m_range->start_offset;
    }

    return caret_position;
}


auto dom::nodes::document::get_the_parent(events::event* event) -> dom::nodes::event_target*
{
    return event->type == "load" or not m_browsing_context ? nullptr : &javascript::realms::relevant_global_object();
}


auto dom::nodes::document::get_m_html_element() const -> html::elements::html_html_element*
{
    // the html element is the document element that is a html_html_element type
    return ext::property_dynamic_cast<html::elements::html_html_element*>(document_element);
}


auto dom::nodes::document::get_m_head_element() const -> html::elements::html_head_element*
{
    // the head element is the first html_head_element that is a child of the html element
    return get_m_html_element()->children->cast_all<html::elements::html_head_element*>().front();
}


auto dom::nodes::document::get_m_title_element() const -> html::elements::html_title_element*
{
    // the title element is the first child of this document that is a html_title_element
    return helpers::trees::descendants(this).cast_all<html::elements::html_title_element*>().front();
}


auto dom::nodes::document::get_m_body_element() const -> html::elements::html_body_element*
{
    // the body element is the first child of this document that is a html_body_element
    return children->cast_all<html::elements::html_body_element*>().front();
}


auto dom::nodes::document::get_compat_mode() const -> ext::string
{
    // the compat mode depends on if the document mode is 'quirks' or not
    return m_mode == "quirks" ? "BackCompat" : "CSS1Compat";
}


auto dom::nodes::document::get_character_set() const -> ext::string
{
    // the character set is the name of the encoding used in the document
    return m_encoding->name;
}


auto dom::nodes::document::get_doctype() const -> dom::nodes::document_type*
{
    // the doctype is the first child of this document that is a doctype node
    return child_nodes->cast_all<document_type*>().front();
}


auto dom::nodes::document::get_document_element() const -> dom::nodes::element*
{
    // the document element is the first child of this document that is an element
    return child_nodes->cast_all<element*>().front();
}


auto dom::nodes::document::get_dir() const -> ext::string
{
    // the dir is a wrapper for the html element's dir attribute
    return get_m_html_element()->dir;
}


auto dom::nodes::document::get_last_modified() const -> ext::string
{
    // TODO
    return "" /* TODO from header */;
}


auto dom::nodes::document::get_body() const -> html::elements::html_body_element*
{
    // the body is the document's body element
    return get_m_body_element();
}


auto dom::nodes::document::get_head() const -> html::elements::html_head_element*
{
    // the head is the document's head element
    return get_m_head_element();
}


auto dom::nodes::document::get_title() const -> ext::string
{
    // the title is the child text content of the title element
    return helpers::trees::child_text_content(get_m_title_element());
}


auto dom::nodes::document::get_images() -> ext::vector<html::elements::html_image_element*>
{
    // the images are the children of this document that are html_image_element nodes
    return helpers::trees::descendants(this).cast_all<html::elements::html_image_element*>();
}


auto dom::nodes::document::get_links() -> ext::vector<html::elements::html_link_element*>
{
    // the links are the children of this document that are html_link_element nodes
    return helpers::trees::descendants(this).cast_all<html::elements::html_link_element*>();
}


auto dom::nodes::document::get_forms() -> ext::vector<html::elements::html_form_element*>
{
    // the forms are the children of this document that are html_form_element nodes
    return helpers::trees::descendants(this).cast_all<html::elements::html_form_element*>();
}


auto dom::nodes::document::get_scripts() -> ext::vector<html::elements::html_script_element*>
{
    // the scripts are the children of this document that are html_script_element nodes which have the href attribute set
    return helpers::trees::descendants(this).cast_all<html::elements::html_script_element*>().filter([](auto* element) {return element->href;});
}


auto dom::nodes::document::set_title(const ext::string& val) -> void
{
    // case for when the document element is a svg element
    if (dynamic_cast<svg::nodes::svg_element*>(document_element))
    {
        // the title element is the first child of the document that is a svg title element
        auto* title_element = document_element->child_nodes->cast_all<svg::nodes::svg_title_element*>().front();

        // if there is no title element, then create an element and insert it as the document element's children's first
        // child
        if (not title_element)
        {
            title_element = helpers::custom_elements::create_an_element(document_element->owner_document, "title", helpers::namespaces::SVG);
            helpers::mutation_algorithms::insert(title_element, document_element, document_element->child_nodes->front());
        }

        // replace all the text in the title element with the new title val
        helpers::node_internals::string_replace_all(val, title_element);
    }

    // case for when the document element is a html element
    else if (document_element->namespace_uri == helpers::namespaces::HTML)
    {
        // do nothing if there is no title or head element in the document
        if (not get_m_title_element() and not get_m_head_element())
            return;

        element* title_element = nullptr;

        // the title element is the html title element if it exists
        if (get_m_title_element())
            auto* title_element = get_m_title_element();

        // otherwise, set the title element to the creation of an element
        else
        {
            title_element = helpers::custom_elements::create_an_element(document_element->owner_document, "title", helpers::namespaces::HTML);
            helpers::mutation_algorithms::append(title_element, get_m_head_element());
        }

        // replace all the text in the title element with the new title val
        helpers::node_internals::string_replace_all(val, title_element);
    }
}


auto dom::nodes::document::set_body(html::elements::html_body_element* val) -> void
{
    // if the new val isn't a html_body_element, then throw a hierarchy request error
    helpers::exceptions::throw_v8_exception(
            "body attribute must be a HTMLBodyElement",
            HIERARCHY_REQUEST_ERR,
            [val] {return dynamic_cast<html::elements::html_body_element*>(val);});

    // if the new val is null and there is no document element, then throw a hierarchy request error
    helpers::exceptions::throw_v8_exception(
            "setting a null body attribute requires a document element to be present",
            HIERARCHY_REQUEST_ERR,
            [val, this] {return not val and not document_element;});

    // append the new val into the document element
    helpers::mutation_algorithms::append(val, document_element);
}


auto dom::nodes::document::set_cookie(const ext::string& val) -> void
{
    // if the document is cookie averse, then return
    if (html::helpers::cookies::is_cookie_averse_document(this))
        return;

    // if the origin is opaque, then throw a security error
    helpers::exceptions::throw_v8_exception(
            "cannot set the cookie of a document that has an opaque origin",
            SECURITY_ERR,
            [this] {return m_origin == "opaque";});

    // set the cookie's new value
    cookie << val;
}


auto dom::nodes::document::set_ready_state(const ext::string& val) -> void
{
    if (ready_state == val) return;
    // TODO : parser stuff
    helpers::event_dispatching::fire_event<>("readyStateChange", this);
}


auto dom::nodes::document::get_scrolling_element() const -> dom::nodes::element*
{
    return m_mode == "quirks" and body
            ? ext::property_dynamic_cast<element*>(body)
            : dynamic_cast<element*>(helpers::trees::root(this));
}


auto dom::nodes::document::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<document>{isolate}
            .ctor<>()
            .inherit<node>()
            .inherit<dom::mixins::document_or_element_node<document>>()
            .inherit<dom::mixins::document_or_shadow_root<document>>()
            .inherit<dom::mixins::parent_node<document>>()
            .inherit<dom::mixins::non_element_parent_node<document>>()
            .inherit<dom::xpath::xpath_evaluator>()
            .inherit<ext::listlike<node*>>()
            .function("createElement", &document::create_element)
            .function("createElementNS", &document::create_element_ns)
            .function("createDocumentFragment", &document::create_document_fragment)
            .function("createTextNode", &document::create_text_node)
            .function("createCDataSectionNode", &document::create_cdata_section_node)
            .function("createComment", &document::create_comment)
            .function("createProcessingInstruction", &document::create_processing_instruction)
            .function("createAttribute", &document::create_attribute)
            .function("createAttributeNS", &document::create_attribute_ns)
            .function("createRange", &document::create_range)
            .function("createNodeIterator", &document::create_node_iterator)
            .function("createTreeWalker", &document::create_tree_walker)
            .function("importNode", &document::import_node)
            .function("adoptNode", &document::adopt_node)
            .function("getElementsByName", &document::get_elements_by_name)
            // .function("open", &document::open)
            .function("close", &document::close)
            // .function("write", &document::write)
            //.function("writeln", &document::writeln)
            .function("hasFocus", &document::has_focus)
            .function("execCommand", &document::exec_command)
            .function("queryCommandEnabled", &document::query_command_enabled)
            .function("queryCommandIndeterm", &document::query_command_indeterm)
            .function("queryCommandState", &document::query_command_state)
            .function("queryCommandSupported", &document::query_command_supported)
            .function("queryCommandValue", &document::query_command_value)
            .function("elementFromPoint", &document::element_from_point)
            .function("elementsFromPoint", &document::elements_from_point)
            .function("caretPositionFromPoint", &document::caret_position_from_point)
            .var("URL", &document::url, true)
            .var("compatMode", &document::compat_mode, true)
            .var("characterSet", &document::character_set, true)
            .var("contentType", &document::content_type, true)
            .var("doctype", &document::doctype, true)
            .var("documentElement", &document::document_element, true)
            .var("implementation", &document::implementation, true)
            .var("domain", &document::domain, true)
            .var("cookie", &document::cookie)
            .var("referrer", &document::referrer, true)
            .var("lastModified", &document::last_modified, true)
            .var("readyState", &document::ready_state, true)
            .var("dir", &document::dir)
            .var("designMode", &document::design_mode)
            .var("title", &document::title)
            .var("location", &document::location)
            .var("body", &document::body)
            .var("head", &document::head, true)
            .var("images", &document::images, true)
            .var("links", &document::links, true)
            .var("forms", &document::forms, true)
            .var("scripts", &document::scripts, true)
            .var("defaultView", &document::default_view, true)
            .var("scrollingElement", &document::scrolling_element)
            .var("namedFlows", &document::named_flows)
            .auto_wrap_objects();
}
