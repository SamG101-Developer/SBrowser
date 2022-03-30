#include "html_element.hpp"

#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/event_dispatching.hpp>
#include <dom/helpers/exceptions.hpp>

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_element::html_element()
        : dom::nodes::element{}
        , mixins::html_or_svg_element<html_element>{}
{
    // set the custom accessors
    inner_text.getter = [this] {return get_inner_text();};
    outer_text.getter = [this] {return get_outer_text();};

    inner_text.setter = [this](auto&& PH1) {set_inner_text(std::forward<decltype(PH1)>(PH1));};
    outer_text.setter = [this](auto&& PH1) {set_outer_text(std::forward<decltype(PH1)>(PH1));};

    offset_top.getter    = [this] {return get_offset_top();};
    offset_left.getter   = [this] {return get_offset_left();};
    offset_width.getter  = [this] {return get_offset_width();};
    offset_height.getter = [this] {return get_offset_height();};

    offset_top.setter    = [this](auto&& PH1) { set_offset_top(std::forward<decltype(PH1)>(PH1));};
    offset_left.setter   = [this](auto&& PH1) { set_offset_left(std::forward<decltype(PH1)>(PH1));};
    offset_width.setter  = [this](auto&& PH1) { set_offset_width(std::forward<decltype(PH1)>(PH1));};
    offset_height.setter = [this](auto&& PH1) { set_offset_height(std::forward<decltype(PH1)>(PH1));};

    // initialize html constructor with boilerplate code
    HTML_CONSTRUCTOR
}


auto html::elements::html_element::click()
        -> void
{
    // do not allow clicks on disabled forms
    if (auto* form_control = ext::property_dynamic_cast<html_form_element*>(parent_element) and helpers::form_controls::is_disabled(form_control))
        return;

    // cannot click during another click (asynchronous processing)
    if (m_click_in_progress_flag)
        return;

    // set the click flag, fire the event, and then unset the click flag
    m_click_in_progress_flag = true;
    dom::helpers::event_dispatching::fire_synthetic_pointer_event("click", this, true);
    m_click_in_progress_flag = false;
}


auto html::elements::html_element::attach_internals()
        -> other::element_internals
{
    // if the 'is' value is empty, then throw a not supported error
    dom::helpers::exceptions::throw_v8_exception(
            "'is' value must be non-null in order to attach internals",
            NOT_SUPPORTED_ERR,
            [this] {return m_is.empty();});

    // get the custom element definition using the empty 'is' value
    auto* definition = dom::helpers::custom_elements::lookup_custom_element_definition(owner_document, namespace_uri, local_name, "");

    // if the definition is null, then throw a not supported error
    dom::helpers::exceptions::throw_v8_exception(
            "definition must be non-null in order to attach internals",
            NOT_SUPPORTED_ERR,
            [definition] {return definition != nullptr;});

    // if the definition has disable_internals enabled, then throw a not supported error
    dom::helpers::exceptions::throw_v8_exception(
            "definition's disableInternals must be false in order to attach internals",
            NOT_SUPPORTED_ERR,
            [definition] {return definition->disable_internals;});

    // if the element already has internals attached, then throw a not supported error
    dom::helpers::exceptions::throw_v8_exception(
            "cannot have already attached internals when attaching internals",
            NOT_SUPPORTED_ERR,
            [this] {return m_attached_internals;});

    // if the definition is not precustomized or custom, then throw a not supported error
    dom::helpers::exceptions::throw_v8_exception(
            "custom element state must be 'precustomized' or 'custom' in order to attach internals",
            NOT_SUPPORTED_ERR,
            [this] {return not ext::vector<ext::string>{"precustomized", "custom"}.contains(m_custom_element_state);});

    // set the attached internals to true, and return a new other::element_internals object with the target set to this
    m_attached_internals = true;
    other::element_internals internals{};
    internals.target = this;
    return internals;
}


auto html::elements::html_element::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<html_element>{isolate}
            .ctor<>()
            .inherit<dom::nodes::element>()
            .inherit<mixins::html_or_svg_element<html_element>>()
            .function("click", &html_element::click)
            .function("attachInternals", &html_element::attach_internals)
            .var("title", html_element::title)
            .var("lang", html_element::lang)
            .var("translate", html_element::translate)
            .var("dir", html_element::dir)
            .var("access_key_label", html_element::access_key_label)
            .var("access_key", html_element::access_key)
            .var("autocapitalize", html_element::autocapitalize)
            .var("inner_text", html_element::inner_text)
            .var("outer_text", html_element::outer_text)
            .var("hidden", html_element::hidden)
            .var("inert", html_element::inert)
            .var("draggable", html_element::draggable)
            .var("spellcheck", html_element::spellcheck)
            .var("offset_parent", html_element::offset_parent)
            .var("offset_top", html_element::offset_top)
            .var("offset_left", html_element::offset_left)
            .var("offset_width", html_element::offset_width)
            .var("offset_height", html_element::offset_height)
            .auto_wrap_objects();
}
