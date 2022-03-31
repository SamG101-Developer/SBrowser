#include "html_meta_element.hpp"

#include <ext/string_switch.hpp>

#include <html/elements/html_head_element.hpp>


html::elements::html_meta_element::html_meta_element() = default;


auto html::elements::html_meta_element::set_http_equiv(
        const ext::string& val)
        -> void
{
    string_switch(val.c_str())
    {
        string_case("content-language"):
        {
            // return if there is no content attribute, or it contains a comma
            if (content->empty() or content->contains(","))
                return;

            // set the candidate to the whitespace left-stripped content, upto the first space
            ext::string input = content;
            input.ltrim(' ');
            auto candidate = input.substring(0, input.find(' '));

            // return if the candidate is empty
            if (candidate.empty())
                return;

            // set the pragma set default language to the candidate
            m_pragma_set_default_language = candidate;
            break;
        }

        string_case("content-encoding"):
        {
            // copy the content string, and check that the trimmed version has teh correct content type and charset/encoding
            ext::string content_string = content;
            assert(content_string.remove(' ') == ext::string{"text/htmlcharset=utf-8"});
            break;
        }

        string_case("default-style"):
            // TODO
            break;

        string_case("refresh"):
            if (content->empty())
                break;

            helpers::document_internals::shared_declarative_refresh_steps(owner_document, content, this);
            assert(content->is_numeric() and std::stod(content->c_str()) >= 0);
            // TODO
            break;

        string_case("set-cookie"):
            break;

        string_case("x-ua-compatible"):
            assert(content == "IE=edge");
            break;

        string_case("content-security-policy"):
            if (not ext::property_dynamic_cast<html_head_element*>(parent_element))
                return;

            if (content->empty())
                return;

            // TODO : csp stuff
    }
}


auto html::elements::html_meta_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    v8pp::class_<html_meta_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .var("name", &html_meta_element::name)
            .var("http_equiv", &html_meta_element::http_equiv)
            .var("content", &html_meta_element::content)
            .var("media", &html_meta_element::media)
            .auto_wrap_objects();
}
