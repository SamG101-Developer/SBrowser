#include "html_parser.hpp"


javascript::parsers::html::html_parser::html_parser(ext::cstring& raw_html)
        : m_raw_html(raw_html)
{}


auto javascript::parsers::html::html_parser::parse() const -> dom::nodes::document*
{
    auto reading_tag_name = false;
    auto reading_attributes = false;
    auto reading_attribute_name = false;
    auto reading_attribute_value = false;

    auto current_tag_name = "";
    auto current_attribute_name = "";
    auto current_attribute_value = "";

    auto current_character = m_raw_html.begin();
    while (current_character != m_raw_html.end())
    {
        if (*current_character == '<')
            reading_tag_name = true;

        else if (*current_character == ' ' and reading_tag_name)
        {
            reading_tag_name = false;

            reading_attributes = true;
            reading_attribute_name = true;
            reading_attribute_value = false;
        }

        else if (*current_character == '=')
        {
            reading_attributes = true;
            reading_attribute_name = false;
            reading_attribute_value = true;
        }

        else if (std::isalpha(*current_character) and reading_attributes)
        {

        }

        else if (std::isalpha(*current_character) and reading_tag_name and not reading_attributes)
            current_tag_name += *current_character;

        else if (*current_character == '>')
            reading_tag_name = false;
    }
}

