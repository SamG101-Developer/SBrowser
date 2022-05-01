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

#include <html/elements/html_body_element.hpp>
#include <html/elements/html_head_element.hpp>
#include <html/elements/html_html_element.hpp>

#include <html/helpers/browsing_context_internals.hpp>
#include <html/helpers/document_internals.hpp>
#include <html/helpers/focus_internals.hpp>

#include <web_idl/types/date.hpp>

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QPointer>


dom::nodes::document::document()
        : node()
        , mixins::non_element_parent_node<document>()
        , mixins::parent_node<document>()
        , mixins::document_or_shadow_root<document>()
        , mixins::document_or_element_node<document>()
        , xpath::xpath_evaluator()
        , ext::listlike<node*>()
{
    // set the custom accessor methods
    compat_mode.getter      = [this] {return get_compat_mode();};
    character_set.getter    = [this] {return get_character_set();};
    doctype.getter          = [this] {return get_doctype();};
    document_element.getter = [this] {return get_document_element();};

    dir.getter           = [this] {return get_compat_mode();};
    design_mode.getter   = [this] {return get_design_mode();};
    last_modified.getter = [this] {return get_last_modified();};
    cookie.getter        = [this] {return get_cookie();};
    body.getter          = [this] {return get_body();};
    head.getter          = [this] {return get_head();};
    title.getter         = [this] {return get_title();};
    images.getter        = [this] {return get_images();};
    links.getter         = [this] {return get_links();};
    forms.getter         = [this] {return get_forms();};
    scripts.getter       = [this] {return get_scripts();};

    design_mode.setter = [this](auto&& PH1) {set_design_mode(std::forward<decltype(PH1)>(PH1));};
    title.setter       = [this](auto&& PH1) {set_title(std::forward<decltype(PH1)>(PH1));};
    body.setter        = [this](auto&& PH1) {set_body(std::forward<decltype(PH1)>(PH1));};
    cookie.setter      = [this](auto&& PH1) {set_cookie(std::forward<decltype(PH1)>(PH1));};
    ready_state.setter = [this](auto&& PH1) {set_ready_state(std::forward<decltype(PH1)>(PH1));};

    // set the property values
    node_type      << DOCUMENT_NODE;
    node_name      << "#document";
    url            << "about:blank";
    content_type   << "application/xml";
    implementation << new other::dom_implementation{};
    ready_state    << "complete";

    // set the attribute values
    m_origin = javascript::realms::surrounding_agent().get<const ext::string&>("origin");
    m_active_document_flags_set = html::internal::sandboxing_flags{};

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
        -> element
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
        -> element
{
    // get the <is> option as a string
    auto is = options.at("is").to<ext::string>();

    // extract the namespace, prefix and local name from the namespace and qualified name
    auto [html_qualified_namespace, prefix, local_name] = helpers::namespaces::validate_and_extract(namespace_, qualified_name);

    // return the created element given the set of options
    return *helpers::custom_elements::create_an_element(const_cast<document*>(this), local_name, html_qualified_namespace, prefix, is, true);
}


auto dom::nodes::document::create_document_fragment() const
        -> document_fragment
{
    // create a new document fragment, and set the owner document to this document
    document_fragment document_fragment_node{};
    document_fragment_node.owner_document = const_cast<document*>(this);

    // return the document fragment
    return document_fragment_node;
}


auto dom::nodes::document::create_text_node(const ext::string& data) const
        -> text
{
    // create a new text node, set the text to data, and set the owner document to this document
    text text_node{};
    text_node.data = data;
    text_node.owner_document = const_cast<document*>(this);

    // return the text node
    return text_node;
}


auto dom::nodes::document::create_cdata_section_node(
        const ext::string& data) const
        -> cdata_section
{
    // if the document type is html, then throw a not supported error
    helpers::exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
            "html documents cannot create cdata_section nodes",
            [this] {return m_type == "html";});

    // if ']]>' is in the data, then throw an invalid character error
    helpers::exceptions::throw_v8_exception<INVALID_CHARACTER_ERR>(
            "cdata_section data cannot contain ']]>'",
            [data] {return data.contains("]]>");});

    // create a new cdata_section node, and set the owner document to this document
    cdata_section cdata_section_node{};
    cdata_section_node.data = data;
    cdata_section_node.owner_document = const_cast<document*>(this);

    // return the cdata_section node
    return cdata_section_node;
}


auto dom::nodes::document::create_comment(
        const ext::string& data) const
        -> comment
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
        -> processing_instruction
{
    // if '?>' is in the data, then throw an invalid character error
    helpers::exceptions::throw_v8_exception<INVALID_CHARACTER_ERR>(
            "processing_instruction data cannot contain '?>'",
            [data] {return data.contains("?>");});

    // create a new comment node, and set the owner document to this document
    processing_instruction processing_instruction_node{};
    processing_instruction_node.target = target;
    processing_instruction_node.data = data;
    processing_instruction_node.owner_document = const_cast<document*>(this);

    // return the processing instruction
    return processing_instruction_node;
}


auto dom::nodes::document::create_attribute(
        const ext::string& local_name) const
        -> attr
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
        -> attr
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
        -> ranges::range
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
        const ulong what_to_show,
        iterators::node_filter* const filter)
        -> iterators::node_iterator
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
        const ulong what_to_show,
        iterators::node_filter* const filter)
        -> iterators::tree_walker
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
        -> node*
{
    // if the node being imported is a document, then throw a not supported error
    helpers::exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
            "cannot import a document node",
            [node] {return dynamic_cast<document*>(node);});

    // if the node being imported is a shadow_root, then throw a not supported error
    helpers::exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
            "cannot import a shadowroot node",
            [node] {return helpers::shadows::is_shadow_root(node);});

    // clone the node into this document
    return helpers::node_internals::clone(node, this, deep);
}


auto dom::nodes::document::adopt_node(
        node* const node)
        -> node*
{
    // if the node being adopted is a document, then throw a not supported error
    helpers::exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
            "cannot adopt a document node",
            [node] {return dynamic_cast<document*>(node);});

    // if the node being adopted is a shadow_root, then throw a hierarchy request error
    helpers::exceptions::throw_v8_exception<HIERARCHY_REQUEST_ERR>(
            "cannot adopt a shadowroot node",
            [node] {return helpers::shadows::is_shadow_root(node);});

    // return nullptr if the node being imported is a document fragment that has a host
    if (dynamic_cast<document_fragment*>(node)->host)
        return nullptr;

    // return the node after running the adopt helper method
    helpers::node_internals::adopt(node, this);
    return node;
}


auto dom::nodes::document::get_elements_by_name(
        const ext::string& element_name) const
        -> ext::vector<node*>
{
    // filter the element descendants, by matching the elements with the same qualified name as element name, and
    // convert them back into nodes before returning the list
    return helpers::trees::descendants(this)
            .cast_all<element*>()
            .filter([element_name](const element* const node) {return node->m_qualified_name == element_name;})
            .cast_all<node*>();
}


auto dom::nodes::document::open() const
        -> document*
{
    // TODO
    return html::helpers::elements::document_open_steps(this);
}


auto dom::nodes::document::open(
        const ext::string& url,
        const ext::string& name,
        const ext::string& features) const
        -> window_proxy*
{
    // TODO
    helpers::exceptions::throw_v8_exception<INVALID_ACCESS_ERR>(
            "cannot open a non-active document",
            [this] {return not helpers::node_internals::is_document_fully_active(const_cast<document*>(this));});

    return html::helpers::elements::window_open_steps(this);
}


auto dom::nodes::document::close() const
        -> void
{
    // TODO
    helpers::exceptions::throw_v8_exception<INVALID_ACCESS_ERR>(
            "cannot close an xml document",
            [this] {return m_type == "xml";});

    helpers::exceptions::throw_v8_exception<INVALID_ACCESS_ERR>(
            "cannot close a document whose dynamic-markup-counter > 0",
            [this] {return m_throw_on_dynamic_markup_insertion_counter > 0;});

    // TODO
}


template <typename ...strings>
auto dom::nodes::document::write(strings... text) const
        -> void
{
    // TODO
    html::helpers::elements::document_write_steps(this, ext::concatenate_strings(text...))
}


template <typename ...strings>
auto dom::nodes::document::writeln(strings... text) const
        -> void
{
    // TODO
    ext::string_vector new_lined_text;
    new_lined_text = {text...};
    new_lined_text.template for_each([](ext::string& string) -> ext::string {string += "\n";});
    write(new_lined_text);
}


auto dom::nodes::document::has_focus() const
        -> bool
{
    return html::helpers::focus_internals::has_focus_steps(this);
}


auto dom::nodes::document::element_from_point(
        const double x,
        const double y) const
        -> element*
{
    // TODO
    return elements_from_point(x, y).front(); // TODO at first found doesn't happen currently
}


auto dom::nodes::document::elements_from_point(
        const double x,
        const double y) const
        -> ext::vector<element*>
{
    if (x < 0 or y < 0)
        return ext::vector<element*>{nullptr};

    return helpers::trees::descendants(this)
            .cast_all<element*>()
            .filter([x, y](element* node) {return node->qt()->geometry().contains(x, y);});
}


auto dom::nodes::document::caret_position_from_point(
        const double x,
        const double y) const
        -> css::cssom_view::other::caret_position*
{

    if (not qt()->widget())
        return nullptr;
    
    if (x < 0 or y < 0)
        return nullptr;

    auto* caret_position = nullptr;
    if (auto* text_insertion_widget = dynamic_cast<QLineEdit*>(qt()->widget()->childAt(x, y))) {
        caret_position = new css::cssom_view::other::caret_position{};
        caret_position->m_range = new ranges::raneg{};
        caret_position->offset_node = caret_position->m_range->start_container;
        caret_position->offset = caret_position->m_range->start_offset;
    }

    return caret_position;
}


auto dom::nodes::document::get_the_parent(
        events::event* event)
        -> event_target*
{
    return event->type == "load" or not m_browsing_context ? nullptr : &javascript::realms::relevant_global_object();
}


auto dom::nodes::document::get_m_html_element() const
        -> html::elements::html_html_element*
{
    // the html element is the document element that is a html_html_element type
    return ext::property_dynamic_cast<html::elements::html_html_element*>(document_element);
}


auto dom::nodes::document::get_m_head_element() const
        -> html::elements::html_head_element*
{
    // the head element is the first html_head_element that is a child of the html element
    return get_m_html_element()->children->cast_all<html::elements::html_head_element*>().front();
}


auto dom::nodes::document::get_m_title_element() const
        -> html::elements::html_title_element*
{
    // the title element is the first child of this document that is a html_title_element
    return helpers::trees::descendants(this).cast_all<html::elements::html_title_element*>().front();
}


auto dom::nodes::document::get_m_body_element() const
        -> html::elements::html_body_element*
{
    // the body element is the first child of this document that is a html_body_element
    return children->cast_all<html::elements::html_body_element*>().front();
}


auto dom::nodes::document::get_compat_mode() const
        -> ext::string
{
    // the compat mode depends on if the document mode is 'quirks' or not
    return m_mode == "quirks" ? "BackCompat" : "CSS1Compat";
}


auto dom::nodes::document::get_character_set() const
        -> ext::string
{
    // the character set is the name of the encoding used in the document
    return m_encoding->name;
}


auto dom::nodes::document::get_doctype() const
        -> document_type*
{
    // the doctype is the first child of this document that is a doctype node
    return child_nodes->cast_all<document_type*>().front();
}


auto dom::nodes::document::get_document_element() const
        -> element*
{
    // the document element is the first child of this document that is an element
    return child_nodes->cast_all<element*>().front();
}


auto dom::nodes::document::get_dir() const
        -> ext::string
{
    // the dir is a wrapper for the html element's dir attribute
    return get_m_html_element()->dir;
}


auto dom::nodes::document::get_default_view() const
        -> window_proxy*
{
    return not m_browsing_context
            ? nullptr
            : m_browsing_context->window_proxy;
}


auto dom::nodes::document::get_last_modified() const
        -> ext::string
{
    // move the date as a string-double ie "1234.5" into a string
    ext::string last_modified_string;
    last_modified >> last_modified_string;

    // convert this string into a webidl data object via a double
    double last_modified_double = std::stod(last_modified_string);
    webidl::types::date last_modified_date{last_modified_double};

    // format the date string from the web idl date type and return it
    auto formatted_date = std::format(
            "{}/{}/{} {}:{}:{}",
            last_modified_date.month(), last_modified_date.day(), last_modified_date.year(),
            last_modified_date.hour(), last_modified_date.minute(), last_modified_date.second());

    return ext::string{formatted_date};
}


auto dom::nodes::document::get_cookie() const
        -> ext::string
{
    // return the empty string if the document is cookie averse
    if (html::helpers::document_internals::is_cookie_averse_document(this))
        return "";

    // if the origin is opaque, then throw a security error
    helpers::exceptions::throw_v8_exception<SECURITY_ERR>(
            "document must have a non-opaque origin in order to access the cookies",
            [this] {return m_origin == "opaque";});

    // return the cookie string
    ext::string cookie_string;
    cookie >> cookie_string;
    return cookie_string;
}


auto dom::nodes::document::get_body() const
        -> html::elements::html_body_element*
{
    // the body is the document's body element or first frameset element if there is no body element
    return get_m_body_element()
            ? get_m_body_element()
            : get_m_html_element()->children->cast_all<html::elements::html_frame_set_elememt*>().front();
}


auto dom::nodes::document::get_head() const
        -> html::elements::html_head_element*
{
    // the head is the document's head element
    return get_m_head_element();
}


auto dom::nodes::document::get_title() const
        -> ext::string
{
    // the title is the child text content of the title element
    return helpers::trees::child_text_content(ext::property_dynamic_cast<svg::elements::svg_element*>(document_element)
            ? document_element->children->cast_all<svg::elements::svg_title_element*>().front()
            : get_m_title_element());
}


auto dom::nodes::document::get_images()
        -> ext::vector<html::elements::html_image_element*>
{
    // the images are the children of this document that are html_image_element nodes
    return helpers::trees::descendants(this).cast_all<html::elements::html_image_element*>();
}


auto dom::nodes::document::get_links()
        -> ext::vector<html::elements::html_link_element*>
{
    // the links are the children of this document that are html_link_element nodes
    return helpers::trees::descendants(this).cast_all<html::elements::html_link_element*>();
}


auto dom::nodes::document::get_forms()
        -> ext::vector<html::elements::html_form_element*>
{
    // the forms are the children of this document that are html_form_element nodes
    return helpers::trees::descendants(this).cast_all<html::elements::html_form_element*>();
}


auto dom::nodes::document::get_scripts()
        -> ext::vector<html::elements::html_script_element*>
{
    // the scripts are the children of this document that are html_script_element nodes which have the href attribute set
    return helpers::trees::descendants(this).cast_all<html::elements::html_script_element*>().filter([](auto* element) {return element->href;});
}


auto dom::nodes::document::set_title(
        const ext::string& val)
        -> void
{
    // case for when the document element is a svg element
    if (dynamic_cast<svg::elements::svg_element*>(document_element))
    {
        // the title element is the first child of the document that is a svg title element
        auto* title_element = document_element->child_nodes->cast_all<svg::elements::svg_title_element*>().front();

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
            title_element = get_m_title_element();

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


auto dom::nodes::document::set_body(
        html::elements::html_body_element* val)
        -> void
{
    // if the new val isn't a html_body_element, then throw a hierarchy request error
    helpers::exceptions::throw_v8_exception<HIERARCHY_REQUEST_ERR>(
            "body attribute must be a HTMLBodyElement or HTMLFrameSetElement",
            [val] {return dynamic_cast<html::elements::html_body_element*>(val);});

    // return if the new body is the same as the current body element
    if (body == val)
        return;

    // replace the current body element with the new body if there is a new body
    if (val)
        helpers::mutation_algorithms::replace(body, parent_element, val);

    // if the new val is null and there is no document element, then throw a hierarchy request error
    helpers::exceptions::throw_v8_exception<HIERARCHY_REQUEST_ERR>(
            "setting a null body attribute requires a document element to be present",
            [val, this] {return not val and not document_element;});

    // append the new val into the document element
    helpers::mutation_algorithms::append(val, document_element);
}


auto dom::nodes::document::set_cookie(
        const ext::string& val)
        -> void
{
    // if the document is cookie averse, then return
    if (html::helpers::document_internals::is_cookie_averse_document(this))
        return;

    // if the origin is opaque, then throw a security error
    helpers::exceptions::throw_v8_exception<SECURITY_ERR>(
            "cannot set the cookie of a document that has an opaque origin",
            [this] {return m_origin == "opaque";});

    // set the cookie's new value
    cookie << val;
}


auto dom::nodes::document::set_ready_state(
        const ext::string& val)
        -> void
{
    if (ready_state == val) return;
    ready_state << val;

    // TODO : html parser association

    helpers::event_dispatching::fire_event<>("readystatechange", this);
}


auto dom::nodes::document::get_scrolling_element() const
        -> dom::nodes::element*
{
    return m_mode == "quirks" and body
            ? ext::property_dynamic_cast<element*>(body)
            : dynamic_cast<element*>(helpers::trees::root(this));
}


auto dom::nodes::document::v8(
        v8::Isolate* isolate) const
        -> ext::any
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
            .function<document*(document::*)() const>("open", &document::open)
            .function<window_proxy*(document::*)(const ext::string&, const ext::string&, const ext::string&) const>("open", &document::open)
            .function("close", &document::close)
            .function("write", &document::write<>)
            .function("writeln", &document::writeln<>)
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
