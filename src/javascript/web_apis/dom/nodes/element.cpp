#include "element.hpp"

#include <ext/infinity.hpp>

#include <dom/helpers/attributes.hpp>
#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/namespaces.hpp>
#include <dom/helpers/node_internals.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/shadow_root.hpp>

#include <QtCore/QObject>
#include <QtWidgets/QWidget>


dom::nodes::element::element()
        : node()
        , mixins::parent_node<element>()
        , mixins::non_document_type_child_node<element>()
        , mixins::child_node<element>()
        , mixins::slottable<element>()
        , mixins::document_or_element<element>()
        , css::cssom_view::mixins::scrolable<element>()
        , css::cssom_view::mixins::geometry_utils<element>() {

    tag_name.get = [this] {return get_tag_name();};
    shadow_root_node.get = [this] {return get_shadow_root();};
    id.set = [this](auto&& PH1) {set_id(std::forward<decltype(PH1)>(PH1));};

    shadow_root_node = nullptr;
    attributes = new ext::vector<attr*>{};
    class_list = new ext::vector<ext::string>{};

    m_custom_element_definition     = nullptr;
    m_custom_element_reaction_queue = {};

    m_rendered_widget = new QWidget{};
}


dom::nodes::element::~element() {
    attributes->clear(true);
    delete attributes;
}


bool
dom::nodes::element::has_attributes() const {
    return not attributes->empty();
}


bool
dom::nodes::element::has_attribute(ext::cstring& name) const {
    ext::string html_qualified_name = helpers::node_internals::is_html(this)
            ? name.new_lowercase()
            : name;

    return not attributes
            ->filter([name](attr* attribute) -> bool {return attribute->name == name;})
            .empty()
}


bool
dom::nodes::element::has_attribute_ns(
        ext::cstring& namespace_,
        ext::cstring& local_name) const {

    return not namespace_
            ->filter([namespace_, local_name](attr* attribute) -> bool {return attribute->namespace_uri == namespace_ and attribute->local_name == local_name;})
            .empty();
}


ext::vector<ext::string>
dom::nodes::element::get_attribute_names() const {

    return attributes->transform<ext::string>([](attr* attribute) -> ext::string {return attibute->name;});
}


ext::string
dom::nodes::element::get_attribute(ext::cstring& qualified_name) const {

    return get_attribute_node(qualified_name)->value;
}


ext::string
dom::nodes::element::get_attribute_ns(
        ext::cstring& namespace_,
        ext::string local_name) const {

    return get_attribute_node_ns(namespace_, local_name)->value;
}


dom::nodes::attr*
dom::nodes::element::get_attribute_node(ext::cstring& qualified_name) const {

    return helpers::attributes::get_attribute_by_name(qualified_name, this);
}


dom::nodes::attr*
dom::nodes::element::get_attribute_node_ns(
        ext::cstring& namespace_,
        ext::cstring& local_name) const {

    return helpers::attributes::get_attribute_by_ns(namespace_, local_name, this);
}


void
dom::nodes::element::set_attribute(
        ext::cstring& qualified_name,
        ext::cstring& value) {

    ext::string html_qualified_name = helpers::node_internals::is_html(qualified_name)
            ? qualified_name.new_lowercase()
            : qualified_name;

    // TODO : move into attr helpers namespacing (set_attribute_by_name)
    attr* attribute = attributes
            ->filter([qualified_name](attr* attribute) -> bool {attribute->name == qualified_name;})
            .front();

    if (not attribute) {
        attribute = new attr{};
        attribute->local_name = qualified_name;
        attribute->value = value;
        attribute->owner_document = owner_document;
        helpers::attributes::append(attribute, this);
    }
    helpers::attributes::change(attribute, value);
}


void
dom::nodes::element::set_attribute_ns(
        ext::cstring& namespace_,
        ext::cstring& qualified_name,
        ext::cstring& value) {

    auto [html_qualified_namespace, prefix, local_name] = helpers::namespaces::validate_and_extract(namespace_, qualified_name);

    ext::string html_qualified_name = helpers::node_iternals::is_html(qualified_name)
            ? qualified_name.new_lowercase()
            : qualified_name;

    // TODO : move into attr helpers namespacing (set_attribute_by_ns)
    attr* attribute = helpers::attributes::get_attribute_by_ns(
            namespace_,
            local_name,
            this);

    if (not attribute) {
        attribute = new attr{};
        attribute->namespace_uri = html_qualified_namespace;
        attribute->prefix = prefix;
        attribute->local_name = local_name;
        attribute->value = value;
        attribute->owner_document = owner_document;
        helpers::attributes::append(attribute, this);
    }
    helpers::attributes::change(attribute, value);
}


dom::nodes::attr*
dom::nodes::element::set_attribute_node(
        attr* attribute) {

    return helpers::attributes::set_attribute(attribute, this);
}


dom::nodes::attr*
dom::nodes::element::set_attribute_node_ns(
        attr* attribute) {

    return set_attribute_node(attribute);
}


void
dom::nodes::element::remove_attribute(ext::cstring& qualified_name) {

    helpers::attributes::remove_attribute_by_name(qualified_name, this);
}


void dom::nodes::element::remove_attribute_ns(
        ext::cstring& namespace_,
        ext::cstring& local_name) {

    helpers::attributes::remove_attribute_by_ns(namespace_, local_name, this);
}


dom::nodes::attr*
dom::nodes::element::remove_attribute_node(attr* attribute) {

    return helpers::attributes::remove_attribute(attribute)
}


dom::nodes::attr*
dom::nodes::element::remove_attribute_node_ns(attr* attribute) {

    return remove_attribute_node(attribute);
}


//not attribute
//        ? set_attribute(qualified_name, "")
//        : remove_attribute(qualified_name);


bool
dom::nodes::element::toggle_attribute(
        ext::cstring& qualified_name,
        bool force) {

    attr* attribute = helpers::attributes::get_attribute_by_name(qualified_name, this);
    return helpers::attibutes::toggle_attribute(attribute);
}


bool
dom::nodes::element::toggle_attribute_ns(
        ext::cstring& namespace_,
        ext::cstring& local_name,
        bool force) {

    attr* attribute = helpers::attributes::get_attribute_by_ns(namespace_, local_name, this);
    return helpers::attributes::toggle_attribute(attribute);
}


dom::nodes::attr*
dom::nodes::element::toggle_attribute_node(
        attr* attribute,
        bool force) {

    return helpers::attributes::toggle_attribute(attribute);
}


dom::nodes::attr*
dom::nodes::element::toggle_attribute_node_ns(
        attr* attribute,
        bool force) {

    return toggle_attribute_node(attribute, force);
}


dom::nodes::shadow_root*
dom::nodes::element::attach_shadow(ext::cstring_any_map& options) {

    helpers::exceptions::throw_v8_exception(
            "cannot attach a shadow to a non-html namespaced element",
            helpers::exceptions::NOT_SUPPORTED_ERR,
            [this] -> bool {return namespace_uri != helpers::namespaces::HTML;});

    helpers::exceptions::throw_v8_exception(
            local_name + " element is incompatible with shadow root attachment",
            helpers::exceptions::NOT_SUPPORTED_ERR,
            [this] -> bool {return not m_local_names.contains(local_name) or not helpers::custom_elements::is_valid_custom_element_name(local_name);});

    helpers::exceptions::throw_v8_exception(
            "cannot attach a shadow root to a shadow root",
            helpers::exceptions::NOT_SUPPORTED_ERR,
            [this] -> bool {return helpers::shadows::is_shadow_root(this)});

    helpers::exceptions::throw_v8_exception(
            "custom element's definition doesn't allow shadow root attachment",
            helpers::exceptions::NOT_SUPPORTED_ERR,
            [this] -> bool {return helpers::custom_elements::lookup_custom_element_definition(owner_document, namespace_uri, local_name, m_is)->disable_shadow and helpers::custom_elements::is_valid_custom_element_name(local_name) or m_is;});

    auto* shadow = new shadow_root{};
    shadow->owner_document = owner_document;
    shadow->host = this;
    shadow->delgates_focus = ext::any_cast<bool>(options.at("delegatesFocus"));
    shadow->slot_assigment = ext::any_cast<bool>(options.at("slotAssignment"));
    shadow->available_to_internals = m_custom_element_state == "custom" or m_custom_element_state == "precustomized";

    shadow_root_node = shadow;
    return shadow;
}


ext::vector<geometry::shapes::dom_rect>
dom::nodes::element::get_client_rects() {

    return ext::vector<QObject*> {render()->children().toVector()}
            .cast_call<QWidget*>()
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

    if (client_rects.all_of(geometry::shapes::dom_rect rect) -> bool {return rect.width == 0.0 or rect.height == 0.0;})
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


QWidget* dom::nodes::element::render() {return qobject_cast<QWidget*>(m_rendered_widget);}


ext::string dom::nodes::element::get_text_content() const {return helpers::trees::descendant_text_content(this);}
ext::string dom::nodes::element::get_tag_name() const {return get_m_html_qualified_uppercase_name();}
dom::nodes::shadow_root* dom::nodes::element::get_shadow_root() const {return shadow_root_node->mode == "closed" ? nullptr : shadow_root_node};

void dom::nodes::element::set_text_content(ext::string val) {helpers::node_internals::replace_all(val, this);}
void dom::nodes::element::set_id(ext::string val) {
    ext::string current_id;
    val >> current_id;
    id << (local_name == id and not namespace_uri->empty() ? val : current_id);
}

ext::string dom::nodes::element::get_m_qualified_name() const {return namespace_uri + ":" + local_name;}
ext::string dom::nodes::element::get_m_html_qualified_uppercase_name() const {return helpers::node_internals::is_html(this) ? get_m_qualified_name().new_uppercase() : get_m_qualified_name()}
