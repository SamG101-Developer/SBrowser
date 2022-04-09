#include "html_option_element.hpp"

#include <dom/helpers/node_internals.hpp>
#include <dom/helpers/trees.hpp>

#include <html/elements/html_opt_group_element.hpp>
#include <html/helpers/input_internals.hpp>


html::elements::html_option_element::html_option_element()
        : html_element()
        , mixins::form_associated<html_option_element>()
{
    // set the custom accessors
    disabled.getter = [this] {return get_disabled();};
    label.getter = [this] {return get_label();};
    text.getter = [this] {return get_text();};
    index.getter = [this] {return get_index();};
    form.getter = [this] {return get_form();};

    selected.setter = [this](auto&& PH1) {set_selected(std::forward<decltype(PH1)>(PH1));};
    text.setter = [this](auto && PH1) {set_text(std::forward<decltype(PH1)>(PH1));};

    selected.deleter = [this] {del_selected();};
}


auto html::elements::html_option_element::get_disabled() const
        -> bool
{
    // this element is disabled if the disabled attribute is set or the parent HTMLOptGroupElement is disabled
    bool disabled_internal;
    disabled >> disabled_internal;

    return disabled_internal or ext::property_dynamic_cast<html_opt_group_element*>(parent_element)->disabled;
}


auto html::elements::html_option_element::get_label() const
        -> ext::string
{
    // the label is the label attribute if set or not the empty string, otherwise the text attribute
    ext::string label_internal;
    label >> label_internal;

    return label->empty() ? text : label_internal;
}


auto html::elements::html_option_element::get_text() const
        -> ext::string
{
    // the text is the concatenation of the descendant text content, stripped and collapsed
    return infra::strip_and_collapse_whitespace(dom::helpers::trees::descendant_text_content(this));
}


auto html::elements::html_option_element::get_index() const
        -> ulong
{
    // the index of this element is the index of it in the ancestor HTMLSelectElement's list of HTMLOptionElements
    auto options_list = helpers::input_internals::list_of_options(helpers::input_internals::select_element_parent(this));
    return options_list.contains(this)?
            options_list.find(this)
            : 0;
}


auto html::elements::html_option_element::get_form() const
        -> html_form_element*
{
    html_form_element* form_internal;
    form >> form_internal;

    return helpers::input_internals::select_element_parent(this) ? form_internal : nullptr;
}


auto html::elements::html_option_element::set_selected(
        bool val)
        -> void
{
    // set the m_selectedness_flag to true if the m_dirtiness_flag isn't set
    if (not m_dirtiness_flag)
        selected << true;

    m_dirtiness_flag = true;
}


auto html::elements::html_option_element::set_text(
        const ext::string& val)
        -> void
{
    // set the text by string replacing all the children (text) nodes with the new text
    dom::helpers::node_internals::string_replace_all(val, this);
}


auto html::elements::html_option_element::del_selected()
        -> void
{
    // set the m_selectedness_flag to false if the m_dirtiness_flag isn't set
    if (not m_dirtiness_flag)
        selected << false;
}
