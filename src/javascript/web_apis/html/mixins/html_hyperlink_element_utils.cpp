#include "html_hyperlink_element_utils.hpp"


html::mixins::html_hyperlink_element_utils::html_hyperlink_element_utils()
        : dom_object{}
{
    // set the custom accessor methods
    href.getter = std::bind(&html_hyperlink_element_utils::get_href, this);

    href.setter = [this](auto && PH1) {set_href(std::forward<decltype(PH1)>(PH1));};
    href.deleter = [this] {set_href("");};
}


auto html::mixins::html_hyperlink_element_utils::get_href()
        -> ext::string
{
    // reinitialize the url
    reinitialize_url();

    // copy out the href
    ext::string href_string;
    href >> href_string;

    // return the href if it exists, otherwise the serialized url if the url exists, otherwise the empty string
    return not href_string.empty() ? href_string : not m_url.empty() ? url::helpers::urls::serialize_url(m_url) : "";
}


auto html::mixins::html_hyperlink_element_utils::set_href(
        const ext::string& val)
        -> void
{
    // set the url and update the href value
    set_the_url();
    href << val;
}


auto html::mixins::html_hyperlink_element_utils::set_the_url()
-> void
{
    if (href->empty())
        m_url = "";
    else
        // TODO : parsing relative to 'owner_document'
        ;
}


auto html::mixins::html_hyperlink_element_utils::reinitialize_url()
        -> void
{
    // return if the murl has an opaque path with the blob scheme
    if (not m_url.empty() and m_url.scheme() == "blob" and url::helpers::urls::is_opaque_path(m_url.path()))
        return;

    // set the url
    set_the_url();
}


auto html::mixins::html_hyperlink_element_utils::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_hyperlink_element_utils>{isolate}
            .inherit<dom_object>()
            .var("href", &html_hyperlink_element_utils::href)
            .var("protocol", &html_hyperlink_element_utils::protocol)
            .var("username", &html_hyperlink_element_utils::username)
            .var("password", &html_hyperlink_element_utils::password)
            .var("host", &html_hyperlink_element_utils::host)
            .var("hostname", &html_hyperlink_element_utils::hostname)
            .var("port", &html_hyperlink_element_utils::port)
            .var("pathname", &html_hyperlink_element_utils::pathname)
            .var("search", &html_hyperlink_element_utils::search)
            .var("hash", &html_hyperlink_element_utils::hash)
            .auto_wrap_objects();
}
