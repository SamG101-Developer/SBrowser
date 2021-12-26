#ifndef SBROWSER_EXPOSE_CPP_TO_JS_HPP
#define SBROWSER_EXPOSE_CPP_TO_JS_HPP

#include <javascript/environment/modules.hpp>

#include <dom/aborting/abort_controller.hpp>
#include <dom/aborting/abort_signal.hpp>

#include <dom/events/event.hpp>
#include <dom/events/custom_event.hpp>

#include <dom/iterators/abstract_iterator.hpp>
#include <dom/iterators/node_filter.hpp>
#include <dom/iterators/node_iterator.hpp>
#include <dom/iterators/tree_walker.hpp>

#include <dom/mixins/child_node.hpp>
#include <dom/mixins/document_or_element_node.hpp>
#include <dom/mixins/document_or_shadow_root.hpp>
#include <dom/mixins/non_document_type_child_node.hpp>
#include <dom/mixins/non_element_parent_node.hpp>
#include <dom/mixins/parent_node.hpp>
#include <dom/mixins/slottable.hpp>

#include <dom/mutations/mutation_observer.hpp>
#include <dom/mutations/mutation_record.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/cdata_section.hpp>
#include <dom/nodes/character_data.hpp>
#include <dom/nodes/comment.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/event_target.hpp>
#include <dom/nodes/node.hpp>
#include <dom/nodes/processing_instruction.hpp>
#include <dom/nodes/shadow_root.hpp>
#include <dom/nodes/text.hpp>
#include <dom/nodes/window.hpp>
#include <dom/nodes/window_proxy.hpp>
#include <dom/nodes/xml_document.hpp>

#include <dom/other/dom_exception.hpp>
#include <dom/other/dom_implementation.hpp>

#include <dom/ranges/abstract_range.hpp>
#include <dom/ranges/range.hpp>
#include <dom/ranges/static_range.hpp>

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/module.hpp>


namespace javascript::interop::expose_cpp_to_js {
    void expose(v8::Isolate* isolate, v8::Persistent<v8::Context>& persistent_context, javascript::environment::modules::module_type module_type);
}


void javascript::interop::expose_cpp_to_js::expose(
        v8::Isolate* isolate,
        v8::Persistent<v8::Context>& persistent_context,
        javascript::environment::modules::module_type module_type) {

    v8::Local<v8::Context> local_context = v8::Local<v8::Context>::New(isolate, persistent_context);
    local_context->Enter();

    v8pp::class_<dom::aborting::abort_controller> v8_abort_controller{isolate};
    v8_abort_controller
            .ctor<>()
            .function("abort", &dom::aborting::abort_controller::abort)
            .var("signal", &dom::aborting::abort_controller::signal)
            .auto_wrap_objects();

    v8pp::class_<dom::aborting::abort_signal> v8_abort_signal{isolate};
    v8_abort_signal
            .inherit<dom::nodes::event_target>()
            .function("abort", &dom::aborting::abort_signal::abort)
            .function("throwIfAborted", &dom::aborting::abort_signal::throw_if_aborted)
            .var("aborted", &dom::aborting::abort_signal::aborted)
            .var("reason", &dom::aborting::abort_signal::reason)
            .auto_wrap_objects();

    v8pp::class_<dom::events::custom_event> v8_custom_event{isolate};
    v8_custom_event
            .ctor<ext::cstring&, ext::cstring_any_map&>()
            .inherit<dom::events::event>()
            .var("detail", &dom::events::custom_event::detail)
            .auto_wrap_objects();

    v8pp::class_<dom::events::event> v8_event{isolate};
    v8_event
            .ctor<ext::cstring&, ext::cstring_any_map&>()

            .static_("NONE", &dom::events::event::NONE)
            .static_("CAPTURING_PHASE", &dom::events::event::CAPTURING_PHASE)
            .static_("AT_TARGET", &dom::events::event::AT_TARGET)
            .static_("BUBBLING_PHASE", &dom::events::event::BUBBLING_PHASE)

            .function("stopImmediatePropagation", &dom::events::event::stop_immediate_propagation)
            .function("stopPropagation", &dom::events::event::stop_propagation)
            .function("preventDefault", &dom::events::event::prevent_default)
            .function("composedPath", &dom::events::event::composed_path)

            .var("type", &dom::events::event::type)
            .var("bubbles", &dom::events::event::bubbles)
            .var("cancelable", &dom::events::event::cancelable)
            .var("composed", &dom::events::event::composed)
            .var("target", &dom::events::event::target)
            .var("currentTarget", &dom::events::event::current_target)
            .var("relatedTarget", &dom::events::event::related_target)
            .var("eventPhase", &dom::events::event::event_phase)
            .var("timeStamp", &dom::events::event::time_stamp)
            .var("isTrusted", &dom::events::event::is_trusted)
            .var("touchTargets", &dom::events::event::touch_targets)
            .var("path", &dom::events::event::path)

            .auto_wrap_objects();

    v8pp::class_<dom::iterators::abstract_iterator> v8_abstract_iterator{isolate};
    v8_abstract_iterator
            .var("root", &dom::iterators::abstract_iterator::root)
            .var("filter", &dom::iterators::abstract_iterator::filter)
            .var("whatToShow", &dom::iterators::abstract_iterator::what_to_show)
            .auto_wrap_objects();

    v8pp::class_<dom::iterators::node_filter> v8_node_filter{isolate};
    v8_node_filter
            .static_("FILTER_ACCEPT", dom::iterators::node_filter::FILTER_ACCEPT)
            .static_("FILTER_SKIP", dom::iterators::node_filter::FILTER_SKIP)
            .static_("FILTER_REJECT", dom::iterators::node_filter::FILTER_REJECT)

            .static_("SHOW_ALL", dom::iterators::node_filter::SHOW_ALL)
            .static_("SHOW_ELEMENT", dom::iterators::node_filter::SHOW_ELEMENT)
            .static_("SHOW_ATTRIBUTE", dom::iterators::node_filter::SHOW_ATTRIBUTE)
            .static_("SHOW_TEXT", dom::iterators::node_filter::SHOW_TEXT)
            .static_("SHOW_CDATA_SECTION", dom::iterators::node_filter::SHOW_CDATA_SECTION)
            .static_("SHOW_PROCESSING_INSTRUCTION", dom::iterators::node_filter::SHOW_PROCESSING_INSTRUCTION)
            .static_("SHOW_COMMENT", dom::iterators::node_filter::SHOW_COMMENT)
            .static_("SHOW_DOCUMENT", dom::iterators::node_filter::SHOW_DOCUMENT)
            .static_("SHOW_DOCUMENT_TYPE", dom::iterators::node_filter::SHOW_DOCUMENT_TYPE)
            .static_("SHOW_DOCUMENT_FRAGMENT", dom::iterators::node_filter::SHOW_DOCUMENT_FRAGMENT)

            .function("acceptNode", &dom::iterators::node_filter::accept_node)
            .auto_wrap_objects();

    v8pp::class_<dom::iterators::node_iterator> v8_node_iterator{isolate};
    v8_node_iterator
            .inherit<dom::iterators::abstract_iterator>()

            .var("referenceNode", &dom::iterators::node_iterator::reference_node)
            .var("pointerBeforeReferenceNode", &dom::iterators::node_iterator::pointer_before_reference_node)

            .function("nextNode", &dom::iterators::node_iterator::next_node)
            .function("previousNode", &dom::iterators::node_iterator::previous_node)

            .auto_wrap_objects();

    v8pp::class_<dom::iterators::tree_walker> v8_tree_walker{isolate};
    v8_tree_walker
            .inherit<dom::iterators::abstract_iterator>()

            .function("parentNode", &dom::iterators::tree_walker::parent_node)
            .function("firstChild", &dom::iterators::tree_walker::first_child)
            .function("lastChild", &dom::iterators::tree_walker::last_child)
            .function("previousSibling", &dom::iterators::tree_walker::previous_sibling)
            .function("nextSibling", &dom::iterators::tree_walker::next_sibling)
            .function("previousNode", &dom::iterators::tree_walker::previous_node)
            .function("nextNode", &dom::iterators::tree_walker::next_node)

            .var("currentNode", &dom::iterators::tree_walker::current_node)

            .auto_wrap_objects();

    v8pp::class_<dom::mixins::child_node<dom::nodes::node>> v8_child_node{isolate};
    v8_child_node
            .auto_wrap_objects();

    v8pp::class_<dom::mixins::document_or_element_node<dom::nodes::node>> v8_document_or_element_node{isolate};
    v8_document_or_element_node
            .function("getElementsByTagName", &dom::mixins::document_or_element_node<dom::nodes::node>::get_elements_by_tag_name)
            .function("getElementsByTagNameNS", &dom::mixins::document_or_element_node<dom::nodes::node>::get_elements_by_tag_name_ns)
            .function("getElementsByClassName", &dom::mixins::document_or_element_node<dom::nodes::node>::get_elements_by_class_name)
            .auto_wrap_objects();

    v8pp::class_<dom::mixins::document_or_shadow_root<dom::nodes::node>> v8_document_or_shadow_root{isolate};
    v8_document_or_shadow_root
            .var("activeElement", &dom::mixins::document_or_shadow_root<dom::nodes::node>::active_element)
            .var("styleSheets", &dom::mixins::document_or_shadow_root<dom::nodes::node>::style_sheets)
            .var("adoptedStyleSheets", &dom::mixins::document_or_shadow_root<dom::nodes::node>::adopted_style_sheets)
            .auto_wrap_objects();

    v8pp::class_<dom::mixins::non_document_type_child_node<dom::nodes::node>> v8_non_document_type_child_node{isolate};
    v8_non_document_type_child_node
            .var("previousElementSibling", &dom::mixins::non_document_type_child_node<dom::nodes::node>::previous_element_sibling)
            .var("nextElementSibling", &dom::mixins::non_document_type_child_node<dom::nodes::node>::next_element_sibling)
            .auto_wrap_objects();

    v8pp::class_<dom::mixins::non_element_parent_node<dom::nodes::node>> v8_non_element_parent_node{isolate};
    v8_non_element_parent_node
            .function("getElementById", &dom::mixins::non_element_parent_node<dom::nodes::node>::get_element_by_id)
            .auto_wrap_objects();

    v8pp::class_<dom::mixins::parent_node<dom::nodes::node>> v8_parent_node{isolate};
    v8_parent_node
//            .function("prepend", &dom::mixins::parent_node<dom::nodes::node>::prepend)
//            .function("append", &dom::mixins::parent_node<dom::nodes::node>::append)
//            .function("replaceChildren", &dom::mixins::parent_node<dom::nodes::node>::replace_children)
            .function("querySelector", &dom::mixins::parent_node<dom::nodes::node>::query_selector)
            .function("querySelectorAll", &dom::mixins::parent_node<dom::nodes::node>::query_selector_all)

            .var("children", &dom::mixins::parent_node<dom::nodes::node>::children)
            .var("firstElementChild", &dom::mixins::parent_node<dom::nodes::node>::first_element_child)
            .var("lastElementChild", &dom::mixins::parent_node<dom::nodes::node>::last_element_child)
            .var("childElementCount", &dom::mixins::parent_node<dom::nodes::node>::child_element_count)
            .auto_wrap_objects();

    v8pp::class_<dom::mixins::slottable<dom::nodes::node>> v8_slottable{isolate};
    v8_slottable
            .var("assignedSlot", &dom::mixins::slottable<dom::nodes::node>::assigned_slot)
            .auto_wrap_objects();

    v8pp::class_<dom::mutations::mutation_observer> v8_mutation_observer{isolate};
    v8_mutation_observer
            .ctor<dom::mutations::mutation_observer::mutation_callback&&>()
            .function("observe", &dom::mutations::mutation_observer::observe)
            .function("disconnect", &dom::mutations::mutation_observer::disconnect)
            .function("takeRecords", &dom::mutations::mutation_observer::take_records)
            .auto_wrap_objects();

    v8pp::class_<dom::mutations::mutation_record> v8_mutation_record{isolate};
    v8_mutation_record
            .var("type", &dom::mutations::mutation_record::type)
            .var("target", &dom::mutations::mutation_record::target)
            .var("addedNodes", &dom::mutations::mutation_record::added_nodes)
            .var("removedNodes", &dom::mutations::mutation_record::removed_nodes)
            .var("previousSibling", &dom::mutations::mutation_record::previous_sibling)
            .var("nextSibling", &dom::mutations::mutation_record::next_sibling)
            .var("attributeName", &dom::mutations::mutation_record::attribute_name)
            .var("attributeNamespace", &dom::mutations::mutation_record::attribute_namespace)
            .var("oldValue", &dom::mutations::mutation_record::old_value)
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::attr> v8_attr{isolate};
    v8_attr
            .inherit<dom::nodes::node>()
            .var("namespaceURI", &dom::nodes::attr::namespace_uri, true)
            .var("prefix", &dom::nodes::attr::prefix, true)
            .var("localName", &dom::nodes::attr::local_name, true)
            .var("name", &dom::nodes::attr::name, true)
            .var("value", &dom::nodes::attr::value)
            .var("ownerElement", &dom::nodes::attr::owner_element, true);

    v8pp::class_<dom::nodes::cdata_section> v8_cdata_section{isolate};
    v8_cdata_section
            .inherit<dom::nodes::text>()
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::character_data> v8_character_data{isolate};
    v8_character_data
            .inherit<dom::nodes::node>()
            .inherit<dom::mixins::child_node<dom::nodes::character_data>>()
            .inherit<dom::mixins::non_document_type_child_node<dom::nodes::character_data>>()

            .function("substringData", &dom::nodes::character_data::substring_data)
            .function("appendData", &dom::nodes::character_data::append_data)
            .function("insertData", &dom::nodes::character_data::insert_data)
            .function("replaceData", &dom::nodes::character_data::replace_data)
            .function("deleteData", &dom::nodes::character_data::delete_data)
            .var("data", &dom::nodes::character_data::data)
            .var("length", &dom::nodes::character_data::length, true)
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::comment> v8_comment{isolate};
    v8_comment
            .ctor<>()
            .ctor<ext::cstring&>()
            .inherit<dom::nodes::character_data>()
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::document> v8_document{isolate};
    v8_document
            .ctor<>()
            .inherit<dom::nodes::node>()
            .inherit<dom::mixins::document_or_element_node<dom::nodes::document>>()
            .inherit<dom::mixins::document_or_shadow_root<dom::nodes::document>>()
            .inherit<dom::mixins::non_element_parent_node<dom::nodes::document>>()

            .function("createElement", &dom::nodes::document::create_element)
            .function("createElementNS", &dom::nodes::document::create_element_ns)
            .function("createDocumentFragment", &dom::nodes::document::create_document_fragment)
            .function("createTextNode", &dom::nodes::document::create_text_node)
            .function("createCDataSectionNode", &dom::nodes::document::create_cdata_section_node)
            .function("createComment", &dom::nodes::document::create_comment)
            .function("createProcessingInstruction", &dom::nodes::document::create_processing_instruction)
            .function("createAttribute", &dom::nodes::document::create_attribute)
            .function("createAttributeNS", &dom::nodes::document::create_attribute_ns)
            .function("createRange", &dom::nodes::document::create_range)
            .function("createNodeIterator", &dom::nodes::document::create_node_iterator)
            .function("createTreeWalker", &dom::nodes::document::create_tree_walker)
            .function("importNode", &dom::nodes::document::import_node)
            .function("adoptNode", &dom::nodes::document::adopt_node)

            .function("getElementsByName", &dom::nodes::document::get_elements_by_name)
//            .function("open", &dom::nodes::document::open)
            .function("close", &dom::nodes::document::close)
//            .function("write", &dom::nodes::document::write)
//            .function("writeln", &dom::nodes::document::writeln)
            .function("hasFocus", &dom::nodes::document::has_focus)
            .function("execCommand", &dom::nodes::document::exec_command)
            .function("queryCommandEnabled", &dom::nodes::document::query_command_enabled)
            .function("queryCommandIndeterm", &dom::nodes::document::query_command_indeterm)
            .function("queryCommandState", &dom::nodes::document::query_command_state)
            .function("queryCommandSupported", &dom::nodes::document::query_command_supported)
            .function("queryCommandValue", &dom::nodes::document::query_command_value)

            .function("elementFromPoint", &dom::nodes::document::element_from_point)
            .function("elementsFromPoint", &dom::nodes::document::elements_from_point)
            .function("caretPositionFromPoint", &dom::nodes::document::caret_position_from_point)

            .var("URL", &dom::nodes::document::url, true)
            .var("compatMode", &dom::nodes::document::compat_mode, true)
            .var("characterSet", &dom::nodes::document::character_set, true)
            .var("contentType", &dom::nodes::document::content_type, true)
            .var("doctype", &dom::nodes::document::doctype, true)
            .var("documentElement", &dom::nodes::document::document_element, true)
            .var("implementation", &dom::nodes::document::implementation, true)

            .var("domain", &dom::nodes::document::domain, true)
            .var("cookie", &dom::nodes::document::cookie)
            .var("referrer", &dom::nodes::document::referrer, true)
            .var("lastModified", &dom::nodes::document::last_modified, true)
            .var("readyState", &dom::nodes::document::ready_state, true)
            .var("dir", &dom::nodes::document::dir)
            .var("designMode", &dom::nodes::document::design_mode)
            .var("title", &dom::nodes::document::title)
            .var("location", &dom::nodes::document::location)
            .var("body", &dom::nodes::document::body)
            .var("head", &dom::nodes::document::head, true)
            .var("images", &dom::nodes::document::images, true)
            .var("links", &dom::nodes::document::links, true)
            .var("forms", &dom::nodes::document::forms, true)
            .var("scripts", &dom::nodes::document::scripts, true)
            .var("defaultView", &dom::nodes::document::default_view, true)

            .var("scrollingElement", &dom::nodes::document::scrolling_element)

            .var("namedFlows", &dom::nodes::document::named_flows)

            .auto_wrap_objects();

    v8pp::class_<dom::nodes::document_fragment> v8_document_fragment{isolate};
    v8_document_fragment
            .ctor<>()
            .inherit<dom::nodes::node>()
            .inherit<dom::mixins::non_element_parent_node<dom::nodes::document_fragment>>()
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::document_type> v8_document_type{isolate};
    v8_document_type
            .inherit<dom::nodes::node>()
            .inherit<dom::mixins::child_node<dom::nodes::character_data>>()

            .var("name", &dom::nodes::document_type::name)
            .var("publicId", &dom::nodes::document_type::public_id)
            .var("systemId", &dom::nodes::document_type::system_id)
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::element> v8_element{isolate};
    v8_element
            .inherit<dom::nodes::node>()
            .inherit<dom::mixins::child_node<dom::nodes::character_data>>()
            .inherit<dom::mixins::document_or_element_node<dom::nodes::element>>()
            .inherit<dom::mixins::non_document_type_child_node<dom::nodes::element>>()
            .inherit<dom::mixins::slottable<dom::nodes::element>>()

            .function("hasAttributes", &dom::nodes::element::has_attributes)
            .function("hasAttribute", &dom::nodes::element::has_attribute)
            .function("hasAttributeNS", &dom::nodes::element::has_attribute_ns)
            .function("getAttributeNames", &dom::nodes::element::get_attribute_names)
            
            .function("getAttribute", &dom::nodes::element::get_attribute)
            .function("getAttributeNS", &dom::nodes::element::get_attribute_ns)
            .function("getAttributeNode", &dom::nodes::element::get_attribute_node)
            .function("hasAttributeNodeNS", &dom::nodes::element::get_attribute_node_ns)

            .function("setAttribute", &dom::nodes::element::set_attribute)
            .function("setAttributeNS", &dom::nodes::element::set_attribute_ns)
            .function("setAttributeNode", &dom::nodes::element::set_attribute_node)
            .function("setAttributeNodeNS", &dom::nodes::element::set_attribute_node_ns)

            .function("removeAttribute", &dom::nodes::element::remove_attribute)
            .function("removeAttributeNS", &dom::nodes::element::remove_attribute_ns)
            .function("removeAttributeNode", &dom::nodes::element::remove_attribute_node)
            .function("removeAttributeNodeNS", &dom::nodes::element::remove_attribute_node_ns)

            .function("toggleAttribute", &dom::nodes::element::toggle_attribute)
            .function("toggleAttributeNS", &dom::nodes::element::toggle_attribute_ns)
            .function("toggleAttributeNode", &dom::nodes::element::toggle_attribute_node)
            .function("toggleAttributeNodeNS", &dom::nodes::element::toggle_attribute_node_ns)

            .function("attachShadow", &dom::nodes::element::attach_shadow)
            .function("closest", &dom::nodes::element::closest)
            .function("matches", &dom::nodes::element::matches)

            .function("getSpatialNavigationContainer", &dom::nodes::element::attach_shadow)
            .function("spatialNavigationSearch", &dom::nodes::element::spatial_navigation_search)
            .function("focusableAreas", &dom::nodes::element::focusable_areas)

            .function("pseudo", &dom::nodes::element::attach_shadow)

            .function("getClientRects", &dom::nodes::element::get_client_rects)
            .function("getBoundingClientRect", &dom::nodes::element::get_bounding_client_rect)
            .function("scrollIntoView", &dom::nodes::element::scroll_into_view)

            .var("namespaceURI", &dom::nodes::element::namespace_uri, true)
            .var("prefix", &dom::nodes::element::prefix, true)
            .var("localName", &dom::nodes::element::local_name, true)
            .var("tagName", &dom::nodes::element::tag_name, true)
            .var("className", &dom::nodes::element::class_name)
            .var("slot", &dom::nodes::element::slot)
            .var("id", &dom::nodes::element::id)
            .var("shadowRoot", &dom::nodes::element::shadow_root_node, true)
            .var("attributes", &dom::nodes::element::attributes, true)
            .var("classList", &dom::nodes::element::class_list, true)

            .var("scrollTop", &dom::nodes::element::scroll_top, true)
            .var("scrollLeft", &dom::nodes::element::scroll_left, true)
            .var("scrollWidth", &dom::nodes::element::scroll_width, true)
            .var("scrollHeight", &dom::nodes::element::scroll_height, true)
            .var("clientTop", &dom::nodes::element::client_top, true)
            .var("clientLeft", &dom::nodes::element::client_left, true)
            .var("clientWidth", &dom::nodes::element::client_width, true)
            .var("clientHeight", &dom::nodes::element::client_height, true)

            .var("parts", &dom::nodes::element::parts)

            .auto_wrap_objects();

    v8pp::class_<dom::nodes::event_target> v8_event_target{isolate};
    v8_event_target
            .ctor<>()
            .function("addEventListener", &dom::nodes::event_target::add_event_listener)
            .function("removeEventListener", &dom::nodes::event_target::remove_event_listener)
            .function("dispatchEvent", &dom::nodes::event_target::dispatch_event)
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::node> v8_node{isolate};
    v8_node
            .inherit<dom::nodes::event_target>()

            .static_("DOCUMENT_POSITION_DISCONNECTED", &dom::nodes::node::DOCUMENT_POSITION_DISCONNECTED)
            .static_("DOCUMENT_POSITION_PRECEDING", &dom::nodes::node::DOCUMENT_POSITION_PRECEDING)
            .static_("DOCUMENT_POSITION_FOLLOWING", &dom::nodes::node::DOCUMENT_POSITION_FOLLOWING)
            .static_("DOCUMENT_POSITION_CONTAINS", &dom::nodes::node::DOCUMENT_POSITION_CONTAINS)
            .static_("DOCUMENT_POSITION_CONTAINED_BY", &dom::nodes::node::DOCUMENT_POSITION_CONTAINED_BY)
            .static_("DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC", &dom::nodes::node::DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC)

            .function("normalize", &dom::nodes::node::normalize)
            .function("hasChildNodes", &dom::nodes::node::has_child_nodes)
            .function("contains", &dom::nodes::node::contains)
            .function("isEqualNode", &dom::nodes::node::is_equal_node)
            .function("isDefaultNamespace", &dom::nodes::node::is_default_namespace)
            .function("lookupPrefix", &dom::nodes::node::lookup_prefix)
            .function("lookupNamespaceURI", &dom::nodes::node::lookup_namespace_uri)
            .function("compareDocumentPosition", &dom::nodes::node::compare_document_position)
            .function("getRootNode", &dom::nodes::node::get_root_node)
            .function("cloneNode", &dom::nodes::node::clone_node)
            .function("insertBefore", &dom::nodes::node::insert_before)
            .function("appendChild", &dom::nodes::node::append_child)
            .function("replaceChild", &dom::nodes::node::replace_child)
            .function("removeChild", &dom::nodes::node::remove_child)

            .var("nodeName", &dom::nodes::node::node_name)
            .var("nodeValue", &dom::nodes::node::node_value)
            .var("textContent", &dom::nodes::node::text_content)
            .var("baseURI", &dom::nodes::node::base_uri)
            .var("isConnected", &dom::nodes::node::is_connected)
            .var("childNodes", &dom::nodes::node::child_nodes)
            .var("parentNode", &dom::nodes::node::parent_node)
            .var("parentElement", &dom::nodes::node::parent_element)
            .var("ownerDocument", &dom::nodes::node::owner_document)
            .var("firstChild", &dom::nodes::node::first_child)
            .var("lastChild", &dom::nodes::node::last_child)
            .var("previousSibling", &dom::nodes::node::previous_sibling)
            .var("nextSibling", &dom::nodes::node::next_sibling)

            .auto_wrap_objects();

    v8pp::class_<dom::nodes::processing_instruction> v8_processing_instruction{isolate};
    v8_processing_instruction
            .inherit<dom::nodes::node>()
            .var("target", &dom::nodes::processing_instruction::target)
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::shadow_root> v8_shadow_root{isolate};
    v8_shadow_root
            .inherit<dom::nodes::document_fragment>()
            .inherit<dom::mixins::document_or_shadow_root<dom::nodes::shadow_root>>()

            .var("mode", &dom::nodes::shadow_root::mode, true)
            .var("delegatesFocus", &dom::nodes::shadow_root::delegates_focus, true)
            .var("slotAssignment", &dom::nodes::shadow_root::slot_assignment, true)
            .var("host", &dom::nodes::shadow_root::host, true)
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::text> v8_text{isolate};
    v8_text
            .ctor<>()
            .ctor<ext::cstring&>()
            .inherit<dom::nodes::character_data>()
            .inherit<dom::mixins::slottable<dom::nodes::text>>()

            .function("splitText", &dom::nodes::text::split_text)
            .var("wholeText", &dom::nodes::text::whole_text, true)
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::window> v8_window{isolate};
    v8_window
            .inherit<dom::nodes::event_target>()
            .inherit<ext::listlike<ext::string>>()

            .function("close", &dom::nodes::window::close)
            .function("stop", &dom::nodes::window::stop)
            .function("focus", &dom::nodes::window::focus)
            .function("open", &dom::nodes::window::open)

            .function("print", &dom::nodes::window::print)
            .function("alert", &dom::nodes::window::alert)
            .function("confirm", &dom::nodes::window::confirm)
            .function("prompt", &dom::nodes::window::prompt)
            .function("postMessage", &dom::nodes::window::post_message)

            .function("move_to", &dom::nodes::window::move_to)
            .function("move_by", &dom::nodes::window::move_by)
            .function("resize_to", &dom::nodes::window::resize_to)
            .function("resize_by", &dom::nodes::window::resize_by)

            .function("getComputedStyle", &dom::nodes::window::get_computed_style)

            .function("navigate", &dom::nodes::window::navigate)

            .var("name", &dom::nodes::window::name)
            .var("status", &dom::nodes::window::status)
            .var("closed", &dom::nodes::window::closed)

            .var("location", &dom::nodes::window::location)
            .var("history", &dom::nodes::window::history)
            .var("customElements", &dom::nodes::window::custom_elements)

            .var("window", &dom::nodes::window::window)
            .var("self", &dom::nodes::window::self)
            .var("ownerDocument", &dom::nodes::window::document)

            .var("location_bar", &dom::nodes::window::location_bar)
            .var("menu_bar", &dom::nodes::window::menu_bar)
            .var("personal_bar", &dom::nodes::window::personal_bar)
            .var("scroll_bars", &dom::nodes::window::scroll_bars)
            .var("status_bar", &dom::nodes::window::status_bar)
            .var("tool_bar", &dom::nodes::window::tool_bar)

            .var("opener", &dom::nodes::window::opener)
            .var("length", &dom::nodes::window::length)
            .var("frameElement", &dom::nodes::window::frame_element)
            .var("frames", &dom::nodes::window::frames)
            .var("top", &dom::nodes::window::top)
            .var("parent", &dom::nodes::window::parent)

            .var("originAgentCluster", &dom::nodes::window::origin_agent_cluster)
            .var("navigator", &dom::nodes::window::navigator)

            .var("innerWidth", &dom::nodes::window::inner_width)
            .var("innerHeight", &dom::nodes::window::inner_height)

            .var("scrollX", &dom::nodes::window::scroll_x)
            .var("scrollY", &dom::nodes::window::scroll_y)
            .var("pageXOffset", &dom::nodes::window::page_x_offset)
            .var("pageYOffset", &dom::nodes::window::page_y_offset)

            .var("screenX", &dom::nodes::window::screen_x)
            .var("screenY", &dom::nodes::window::screen_y)
            .var("outerWidth", &dom::nodes::window::outer_width)
            .var("outerHeight", &dom::nodes::window::outer_height)
            .var("devicePixelWidth", &dom::nodes::window::device_pixel_width)

            .var("screen", &dom::nodes::window::screen)

            .auto_wrap_objects();

    v8pp::class_<dom::nodes::window_proxy> v8_window_proxy{isolate};
    v8_window_proxy
            .function("[[GetPrototypeOf]]", &dom::nodes::window_proxy::get_prototype_of)
            .function("[[SetPrototypeOf]]", &dom::nodes::window_proxy::set_prototype_of)
            .function("[[IsExtensiblePrototypeOf]]", &dom::nodes::window_proxy::is_extensible_prototype_of)
            .function("[[PreventExtensions]]", &dom::nodes::window_proxy::prevent_extensions)
            .function("[[GetOwnProperty]]", &dom::nodes::window_proxy::get_own_property)
            .function("[[DefineProperty]]", &dom::nodes::window_proxy::define_property)
            .function("[[Get]]", &dom::nodes::window_proxy::get)
            .function("[[Set]]", &dom::nodes::window_proxy::set)
            .function("[[Delete]]", &dom::nodes::window_proxy::delete_)
            .function("[[OwnPropertyKeys]]", &dom::nodes::window_proxy::own_property_keys)

            .var("[[Window]]", &dom::nodes::window_proxy::s_window)
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::xml_document> v8_xml_document{isolate};
    v8_xml_document
            .inherit<dom::nodes::document>()
            .auto_wrap_objects();

    v8pp::class_<dom::other::dom_exception> v8_dom_exception{isolate};
    v8_dom_exception
            .ctor<ext::cstring&, exception_type>()
            .var("message", &dom::other::dom_exception::message)
            .var("type", &dom::other::dom_exception::type)

            .static_("INDEX_SIZE_ERR", exception_type::INDEX_SIZE_ERR)
            .static_("DOMSTRING_SIZE_ERR", exception_type::DOMSTRING_SIZE_ERR)
            .static_("HIERARCHY_REQUEST_ERR", exception_type::HIERARCHY_REQUEST_ERR)
            .static_("WRONG_DOCUMENT_ERR", exception_type::WRONG_DOCUMENT_ERR)
            .static_("INVALID_CHARACTER_ERR", exception_type::INVALID_CHARACTER_ERR)
            .static_("NO_DATA_ALLOWED_ERR", exception_type::NO_DATA_ALLOWED_ERR)
            .static_("NO_MODIFICATION_ALLOWED_ERR", exception_type::NO_MODIFICATION_ALLOWED_ERR)
            .static_("NOT_FOUND_ERR", exception_type::NOT_FOUND_ERR)
            .static_("NOT_SUPPORTED_ERR", exception_type::NOT_SUPPORTED_ERR)
            .static_("INUSE_ATTRIBUTE_ERR", exception_type::INUSE_ATTRIBUTE_ERR)
            .static_("INVALID_STATE_ERR", exception_type::INVALID_STATE_ERR)
            .static_("SYNTAX_ERR", exception_type::SYNTAX_ERR)
            .static_("INVALID_MODIFICATION_ERR", exception_type::INVALID_MODIFICATION_ERR)
            .static_("NAMESPACE_ERR", exception_type::NAMESPACE_ERR)
            .static_("INVALID_ACCESS_ERR", exception_type::INVALID_ACCESS_ERR)
            .static_("VALIDATION_ERR", exception_type::VALIDATION_ERR)
            .static_("TYPE_MISMATCH_ERR", exception_type::TYPE_MISMATCH_ERR)
            .static_("SECURITY_ERR", exception_type::SECURITY_ERR)
            .static_("NETWORK_ERR", exception_type::NETWORK_ERR)
            .static_("ABORT_ERR", exception_type::ABORT_ERR)
            .static_("URL_MISMATCH_ERR", exception_type::URL_MISMATCH_ERR)
            .static_("QUOTA_EXCEEDED_ERR", exception_type::QUOTA_EXCEEDED_ERR)
            .static_("TIMEOUT_ERR", exception_type::TIMEOUT_ERR)
            .static_("INVALID_NODE_TYPE_ERR", exception_type::INVALID_NODE_TYPE_ERR)
            .static_("DATA_CLONE_ERR", exception_type::DATA_CLONE_ERR)
            .static_("ENCODING_ERR", exception_type::ENCODING_ERR)
            .static_("NOT_READABLE_ERR", exception_type::NOT_READABLE_ERR)
            .static_("UNKNOWN_ERR", exception_type::UNKNOWN_ERR)
            .static_("CONSTRAINT_ERR", exception_type::CONSTRAINT_ERR)
            .static_("DATA_ERR", exception_type::DATA_ERR)
            .static_("TRANSACTION_INACTIVE_ERR", exception_type::TRANSACTION_INACTIVE_ERR)
            .static_("READONLY_ERR", exception_type::READONLY_ERR)
            .static_("VERSION_ERR", exception_type::VERSION_ERR)
            .static_("OPERATION_ERR", exception_type::OPERATION_ERR)
            .static_("NOT_ALLOWED_ERR", exception_type::NOT_ALLOWED_ERR)

            .auto_wrap_objects();

    v8pp::class_<dom::other::dom_implementation> v8_dom_implementation{isolate};
    v8_dom_implementation
            .function("createDocumentType", &dom::other::dom_implementation::create_document_type)
            .function("createDocument", &dom::other::dom_implementation::create_document)
            .function("createHTMLDocument", &dom::other::dom_implementation::create_html_document)
            .auto_wrap_objects();

    v8pp::class_<dom::ranges::abstract_range> v8_abstract_range{isolate};
    v8_abstract_range
            .var("collapsed", &dom::ranges::abstract_range::collapsed)
            .var("startContainer", &dom::ranges::abstract_range::start_container)
            .var("startOffset", &dom::ranges::abstract_range::start_offset)
            .var("endContainer", &dom::ranges::abstract_range::end_container)
            .var("endOffset", &dom::ranges::abstract_range::end_offset)
            .auto_wrap_objects();

    v8pp::class_<dom::ranges::range> v8_range{isolate};
    v8_range
            .ctor<>()
            .inherit<dom::ranges::abstract_range>()

            .static_("START_TO_START", dom::ranges::range::START_TO_START)
            .static_("START_TO_END", dom::ranges::range::START_TO_END)
            .static_("END_TO_END", dom::ranges::range::END_TO_END)
            .static_("END_TO_START", dom::ranges::range::END_TO_START)

            .function("setStart", &dom::ranges::range::set_start)
            .function("setStartAfter", &dom::ranges::range::set_start_after)
            .function("setStartBefore", &dom::ranges::range::set_start_before)

            .function("setEnd", &dom::ranges::range::set_end)
            .function("setEndAfter", &dom::ranges::range::set_end_after)
            .function("setEndBefore", &dom::ranges::range::set_end_before)

            .function("insertNode", &dom::ranges::range::insert_node)
            .function("intersectsNode", &dom::ranges::range::intersects_node)
            .function("selectNode", &dom::ranges::range::select_node)
            .function("selectNodeContents", &dom::ranges::range::select_node_contents)

            .function("compareBoundaryPoints", &dom::ranges::range::compare_boundary_points)
            .function("comparePoint", &dom::ranges::range::compare_point)

            .function("extractContents", &dom::ranges::range::extract_contents)
            .function("cloneContents", &dom::ranges::range::clone_contents)
            .function("deleteContents", &dom::ranges::range::delete_contents)
            .function("surroundContents", &dom::ranges::range::surround_contents)

            .function("collapse", &dom::ranges::range::collapse)
            .function("cloneRange", &dom::ranges::range::clone_range)
            .function("isPointInRange", &dom::ranges::range::is_point_in_range)

            .function("toJSON", &dom::ranges::range::to_json)

            .var("commonAncestorContainer", &dom::ranges::range::common_ancestor_container)

            .auto_wrap_objects();

    v8pp::class_<dom::ranges::static_range> v8_static_range{isolate};
    v8_static_range
            .ctor<ext::cstring_any_map&>()
            .inherit<dom::ranges::abstract_range>()
            .auto_wrap_objects();

    v8pp::module v8_module{isolate};
    v8::Local<v8::String> module_name;

    switch(module_type) {
        case javascript::environment::modules::module_type::window: {
            v8_module
                    .class_("AbortController", v8_abort_controller)
                    .class_("AbortSignal", v8_abort_signal)

                    .class_("CustomEvent", v8_custom_event)
                    .class_("Event", v8_event)

                    .class_("NodeFilter", v8_node_filter)
                    .class_("NodeIterator", v8_node_iterator)
                    .class_("TreeWalker", v8_tree_walker)

                    .class_("MutationObserver", v8_mutation_observer)
                    .class_("MutationRecord", v8_mutation_record)

                    .class_("Attr", v8_attr)
                    .class_("CDataSection", v8_cdata_section)
                    .class_("CharacterData", v8_character_data)
                    .class_("Comment", v8_comment)
                    .class_("Document", v8_document)
                    .class_("DocumentFragment", v8_document_fragment)
                    .class_("DocumentType", v8_document_type)
                    .class_("EventTarget", v8_event_target)
                    .class_("Node", v8_node)
                    .class_("ProcessingInstruction", v8_processing_instruction)
                    .class_("ShadowRoot", v8_shadow_root)
                    .class_("Text", v8_text)
                    .class_("Window", v8_window)
                    .class_("WindowProxy", v8_window_proxy)
                    .class_("XmlDocument", v8_xml_document)

                    .class_("DomException", v8_dom_exception)
                    .class_("DomImplementation", v8_dom_implementation)

                    .class_("AbstractRange", v8_abstract_range)
                    .class_("Range", v8_range)
                    .class_("StaticRange", v8_static_range);

            module_name = v8::String::NewFromUtf8(isolate, "Window").ToLocalChecked();
        }

        case javascript::environment::modules::module_type::worker: {
            v8_module
                    .class_("CustomEvent", v8_custom_event)
                    .class_("Event", v8_event)
                    .class_("EventTarget", v8_event_target)
                    .class_("DomException", v8_dom_exception);

            module_name = v8::String::NewFromUtf8(isolate, "Worker").ToLocalChecked();
        }

        case javascript::environment::modules::module_type::audio_worklet: {
            v8_module
                    .class_("Event", v8_event)

                    .class_("EventTarget", v8_event_target);

            module_name = v8::String::NewFromUtf8(isolate, "AudioWorklet").ToLocalChecked();
        }

        default:
            module_name = v8::String::NewFromUtf8(isolate, "Unknown").ToLocalChecked();
    }

    local_context->Global()->Set(local_context, module_name, v8_module.new_instance());
}


#endif //SBROWSER_EXPOSE_CPP_TO_JS_HPP
