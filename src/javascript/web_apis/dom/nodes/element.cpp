#include "element.hpp"

#include <ext/types/infinity.hpp>

#include <dom/helpers/attributes.hpp>
#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/namespaces.hpp>
#include <dom/helpers/node_internals.hpp>
#include <dom/helpers/shadows.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/shadow_root.hpp>

#include <geometry/shapes/dom_rect.hpp>

#include <QtWidgets/QWidget>


dom::nodes::element::element()
        : node()
        , mixins::parent_node<element>()
        , mixins::non_document_type_child_node<element>()
        , mixins::child_node<element>()
        , mixins::slottable<element>()
        , mixins::document_or_element_node<element>()
//        , css::cssom_view::mixins::scrollable<element>()
//        , css::cssom_view::mixins::geometry_utils<element>() {
{
    // set the custom accessors
    tag_name.get = [this] {return get_tag_name();};
    shadow_root_node.get = [this] {return get_shadow_root();};

    // set the properties
    node_type << ELEMENT_NODE;
    shadow_root_node << nullptr;
    attributes << new ext::vector<attr*>{};
    class_list << new ext::vector<ext::string>{};

    // set the attributes
    m_custom_element_definition = nullptr;
    m_custom_element_reaction_queue = {};

    // create the widget representation
    m_rendered_widget = new QWidget{};
}


dom::nodes::element::~element()
{
    // delete all the attributes when the element is deleted
    attributes->clear();
    delete attributes;
}


bool
dom::nodes::element::has_attributes() const
{
    // return if the attributes list is not empty
    return not attributes->empty();
}


ext::vector<ext::string>
dom::nodes::element::get_attribute_names() const
{
    // return a list of attributes transformed into their names
    return attributes->transform<ext::string>([](attr* attribute) -> ext::string {return attribute->name;});
}


bool
dom::nodes::element::has_attribute(ext::cstring& name) const
{
    // if this element is a html element then set the html qualified name to lowercase
    ext::string html_qualified_name = helpers::node_internals::is_html(this)
            ? name.new_lowercase()
            : name;

    // return if the list of attributes filtered to have a matching name contains any elements or not
    return not attributes->filter([name](attr* attribute) {return attribute->name == name;}).empty();
}


bool
dom::nodes::element::has_attribute_ns(
        ext::cstring& namespace_,
        ext::cstring& local_name) const
{
    // return if the list of attributes filtered to have a matching namespace and local_name contains any elements or not
    return not attributes->filter([namespace_, local_name](attr* attribute) {return attribute->namespace_uri == namespace_ and attribute->local_name == local_name;}).empty();
}


bool
dom::nodes::element::has_attribute_node(attr* attribute)
{
    // return if the attribute is in the attributes list
    return attributes->contains(attribute);
}


bool
dom::nodes::element::has_attribute_node_ns(attr* attribute)
{
    // return if the attribute is in the attributes list
    return attributes->contains(attribute);
}


ext::string
dom::nodes::element::get_attribute(ext::cstring& qualified_name) const
{
    // return the value of an attribute that whose name matches qualified_name
    return get_attribute_node(qualified_name)->value;
}


ext::string
dom::nodes::element::get_attribute_ns(ext::cstring& namespace_, ext::string local_name) const
{
    // return the value of an attribute that whose namespace and local_name matches namespace and local_name
    return get_attribute_node_ns(namespace_, local_name)->value;
}


dom::nodes::attr*
dom::nodes::element::get_attribute_node(ext::cstring& qualified_name) const
{
    // return an attribute whose name matches qualified_name
    return helpers::attributes::get_attribute_by_name(qualified_name, this);
}


dom::nodes::attr*
dom::nodes::element::get_attribute_node_ns(
        ext::cstring& namespace_,
        ext::cstring& local_name) const
{
    // return an attribute whose namespace and local_name matches namespace_ and local_name
    return helpers::attributes::get_attribute_by_ns(namespace_, local_name, this);
}


void
dom::nodes::element::set_attribute(
        ext::cstring& qualified_name,
        ext::cstring& value)
{
    // set the value to an attribute whose name matches the qualified name
    helpers::attributes::set_attribute_by_name(qualified_name, this, value);
}


void
dom::nodes::element::set_attribute_ns(
        ext::cstring& namespace_,
        ext::cstring& qualified_name,
        ext::cstring& value)
{
    // set the value to an attribute whose namespace, name matches the namespace_ and qualified_name
    helpers::attributes::set_attribute_by_ns(namespace_, qualified_name, this, value);
}


dom::nodes::attr*
dom::nodes::element::set_attribute_node(attr* attribute)
{
    // set an attribute by a node
    return helpers::attributes::set_attribute(attribute, this);
}


dom::nodes::attr*
dom::nodes::element::set_attribute_node_ns(attr* attribute)
{
    // set an attribute by a node (same as set_attribute_node, included for uniform function set)
    return set_attribute_node(attribute);
}


void
dom::nodes::element::remove_attribute(ext::cstring& qualified_name)
{
    // remove an attribute whose name matches qualified_name
    helpers::attributes::remove_attribute_by_name(qualified_name, this);
}


void dom::nodes::element::remove_attribute_ns(
        ext::cstring& namespace_,
        ext::cstring& local_name)
{
    // remove an attribute whose namespace and local_name matches namespace_ and local_name 
    helpers::attributes::remove_attribute_by_ns(namespace_, local_name, this);
}


dom::nodes::attr*
dom::nodes::element::remove_attribute_node(attr* attribute)
{
    // remove an attribute by a node
    return helpers::attributes::remove_attribute(attribute, this);
}


dom::nodes::attr*
dom::nodes::element::remove_attribute_node_ns(attr* attribute)
{
    // remove an attribute by a node (same as remove_attribute_node, included for uniform function set)
    return remove_attribute_node(attribute);
}



bool
dom::nodes::element::toggle_attribute(
        ext::cstring& qualified_name,
        bool force)
{
    // toggle an attribute whose name matches qualified_name
    return helpers::attributes::toggle_attribute_by_name(qualified_name, this, force);
}


bool
dom::nodes::element::toggle_attribute_ns(
        ext::cstring& namespace_,
        ext::cstring& local_name,
        bool force)
{
    // toggle an attribute whose namespace and local_name matches namespace_ and local_name
    return helpers::attributes::toggle_attribute_by_ns(namespace_, local_name, this, force);
}


dom::nodes::attr*
dom::nodes::element::toggle_attribute_node(
        attr* attribute,
        bool force)
{
    // toggle an attribute by a node
    return helpers::attributes::toggle_attribute(attribute, force);
}


dom::nodes::attr*
dom::nodes::element::toggle_attribute_node_ns(
        attr* attribute,
        bool force)
{
    // toggle the attribute by a node (same as toggle_attribute_node, included for uniform function set)
    return toggle_attribute_node(attribute, force);
}


dom::nodes::shadow_root*
dom::nodes::element::attach_shadow(ext::cstring_any_map& options)
{
    // if the namespace is not html, then throw a not supported error
    helpers::exceptions::throw_v8_exception(
            "cannot attach a shadow to a non-html namespaced element",
            NOT_SUPPORTED_ERR,
            [this] {return namespace_uri != helpers::namespaces::HTML;});

    // if the local name is unknown, or not a known custom element, then throw a not supported error
    helpers::exceptions::throw_v8_exception(
            local_name + " element is incompatible with shadow root attachment",
            NOT_SUPPORTED_ERR,
            [this] {return not m_shadow_attachable_local_names.contains(local_name) or not helpers::custom_elements::is_valid_custom_element_name(local_name);});

    // if this element is a shadow root, then throw a not supported error
    helpers::exceptions::throw_v8_exception(
            "cannot attach a shadow root to a shadow root",
            NOT_SUPPORTED_ERR,
            [this] {return helpers::shadows::is_shadow_root(this);});

    // if this node is a custom noe that has shadows disabled, then throw a not supported error
    helpers::exceptions::throw_v8_exception(
            "custom element's definition doesn't allow shadow root attachment",
            NOT_SUPPORTED_ERR,
            [this] {return helpers::custom_elements::is_valid_custom_element_name(local_name) and helpers::custom_elements::lookup_custom_element_definition(owner_document, namespace_uri, local_name, m_is)->disable_shadow or m_is;});

    // create a new shadow root, and set the owner document to this document
    auto* shadow = new shadow_root{};
    shadow->host = this;
    shadow->delegates_focus = options.at("delegatesFocus").to<bool>();
    shadow->slot_assignment = options.at("slotAssignment").to<ext::string>();
    shadow->owner_document = owner_document;
    shadow->m_available_to_internals = m_custom_element_state == "custom" or m_custom_element_state == "precustomized";

    // set this element's shadow root to the shadow, and return the shadow
    shadow_root_node = shadow;
    return shadow;
}


ext::vector<geometry::shapes::dom_rect>
dom::nodes::element::get_client_rects()
{
    auto children = render()->children().toVector();

    return ext::vector<QObject*>{children.begin(), children.end()}
            .cast_all<QWidget*>()
            .transform<geometry::shapes::dom_rect>([](QWidget* widget) {return geometry::shapes::dom_rect{
                    (double)widget->geometry().left(),
                    (double)widget->geometry().top(),
                    (double)widget->geometry().right(),
                    (double)widget->geometry().bottom()
            };});
}


geometry::shapes::dom_rect
dom::nodes::element::get_bounding_client_rect() {
    auto client_rects = get_client_rects();
    if (client_rects.empty())
        return geometry::shapes::dom_rect{0, 0, 0, 0};

    if (client_rects.all_of([](const geometry::shapes::dom_rect& rect) {return rect.width == 0.0 or rect.height == 0.0;}))
        return client_rects.front();

    double furthest_left = ext::infinity<double>{};
    double furthest_top = ext::infinity<double>{};
    double furthest_right = 0.0;
    double furthest_bottom = 0.0;

    for (auto rect: client_rects) {
        furthest_left = std::min(furthest_left, (double)rect.x);
        furthest_top = std::min(furthest_top, (double)rect.y);
        furthest_right = std::max(furthest_right, (double)rect.width + rect.x);
        furthest_bottom = std::max(furthest_bottom, (double)rect.height + rect.y);
    }

    return geometry::shapes::dom_rect{furthest_left, furthest_top, furthest_right - furthest_left, furthest_bottom - furthest_top};
}


INLINE ext::string
dom::nodes::element::get_text_content() const
{
    // the text content is the descendant text contents of this node
    return helpers::trees::descendant_text_content(this);
}


INLINE ext::string
dom::nodes::element::get_tag_name() const
{
    // the tag name is the html qualified name
    return get_m_html_qualified_uppercase_name();
}


INLINE dom::nodes::shadow_root*
dom::nodes::element::get_shadow_root() const
{
    // the shadow root is the shadow root if the mode option is closed, otherwise null (access from javascript)
    return shadow_root_node->mode == "closed" ? nullptr : shadow_root_node;
}


INLINE void
dom::nodes::element::set_text_content(ext::cstring& val)
{
    // set the text_content by replacing the data with the val
    helpers::node_internals::string_replace_all(val, this);
}


INLINE ext::string
dom::nodes::element::get_m_qualified_name() const
{
    // the qualified name is the namespace and local name joined with a colon
    return namespace_uri + ext::string{":"} + local_name;
}


INLINE ext::string
dom::nodes::element::get_m_html_qualified_uppercase_name() const
{
    // the html qualified uppercase name is the qualified name, in uppercase if the element is html
    return helpers::node_internals::is_html(this) ? get_m_qualified_name().new_uppercase() : get_m_qualified_name();
}


QWidget* dom::nodes::element::render()
{
    return qobject_cast<QWidget*>(m_rendered_widget);
}


ext::any dom::nodes::element::v8(v8::Isolate* isolate) const
{
    return v8pp::class_<element>{isolate}
            .inherit<node>()
            .inherit<dom::mixins::child_node<element>>()
            .inherit<dom::mixins::document_or_element_node<element>>()
            .inherit<dom::mixins::non_document_type_child_node<element>>()
            .inherit<dom::mixins::slottable<element>>()

            .function("hasAttributes", &element::has_attributes)
            .function("getAttributeNames", &element::get_attribute_names)

            .function("hasAttribute", &element::has_attribute)
            .function("hasAttributeNS", &element::has_attribute_ns)
            .function("hasAttributeNode", &element::has_attribute_node)
            .function("hasAttributeNodeNS", &element::has_attribute_node_ns)

            .function("getAttribute", &element::get_attribute)
            .function("getAttributeNS", &element::get_attribute_ns)
            .function("getAttributeNode", &element::get_attribute_node)
            .function("hasAttributeNodeNS", &element::get_attribute_node_ns)

            .function("setAttribute", &element::set_attribute)
            .function("setAttributeNS", &element::set_attribute_ns)
            .function("setAttributeNode", &element::set_attribute_node)
            .function("setAttributeNodeNS", &element::set_attribute_node_ns)

            .function("removeAttribute", &element::remove_attribute)
            .function("removeAttributeNS", &element::remove_attribute_ns)
            .function("removeAttributeNode", &element::remove_attribute_node)
            .function("removeAttributeNodeNS", &element::remove_attribute_node_ns)

            .function("toggleAttribute", &element::toggle_attribute)
            .function("toggleAttributeNS", &element::toggle_attribute_ns)
            .function("toggleAttributeNode", &element::toggle_attribute_node)
            .function("toggleAttributeNodeNS", &element::toggle_attribute_node_ns)

            .function("attachShadow", &element::attach_shadow)
            .function("closest", &element::closest)
            .function("matches", &element::matches)

            .function("getSpatialNavigationContainer", &element::attach_shadow)
            .function("spatialNavigationSearch", &element::spatial_navigation_search)
            .function("focusableAreas", &element::focusable_areas)

            .function("pseudo", &element::attach_shadow)

            .function("getClientRects", &element::get_client_rects)
            .function("getBoundingClientRect", &element::get_bounding_client_rect)
            .function("scrollIntoView", &element::scroll_into_view)

            .var("namespaceURI", &element::namespace_uri, true)
            .var("prefix", &element::prefix, true)
            .var("localName", &element::local_name, true)
            .var("tagName", &element::tag_name, true)
            .var("className", &element::class_name)
            .var("slot", &element::slot)
            .var("id", &element::id)
            .var("shadowRoot", &element::shadow_root_node, true)
            .var("attributes", &element::attributes, true)
            .var("classList", &element::class_list, true)

            .var("scrollTop", &element::scroll_top, true)
            .var("scrollLeft", &element::scroll_left, true)
            .var("scrollWidth", &element::scroll_width, true)
            .var("scrollHeight", &element::scroll_height, true)
            .var("clientTop", &element::client_top, true)
            .var("clientLeft", &element::client_left, true)
            .var("clientWidth", &element::client_width, true)
            .var("clientHeight", &element::client_height, true)

            .var("parts", &element::parts)

            .auto_wrap_objects();
}
