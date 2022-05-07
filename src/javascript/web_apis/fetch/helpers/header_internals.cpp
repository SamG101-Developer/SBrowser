#include "header_internals.hpp"

#include <ext/string.hpp>


auto fetch::helpers::header_internals::get_structured_field(
        const header_name_t& name,
        const header_value_t& type,
        const header_list_t& list)
        -> ext::optional<header_name_t>
{
    // check that the 'type' is valid
    assert(ext::string_vector{"dictionary", "list", "item"}.contains(type));

    // get the value of the name in the list
    auto value = get(name, list);

    // return the null if the object doesn't exist, otherwise the parsed result of the value
    return value.has_value() ? ext::optional(parse_structured_fields(*value, type)) : std::nullopt;
}


auto fetch::helpers::header_internals::set_structured_field(
        const header_t& header,
        header_list_t& list)
        -> void
{
    // TODO : serialize header.second
    // append the header to the list
    list.append(header);
}


auto fetch::helpers::header_internals::contains_header(
        const header_name_t& name,
        const header_list_t& list)
        -> bool
{
    // case-insensitive match to check if a name already exists in the list TODO : tidy
    return list.any_of(
            [name](const header_t& compare_header)
            {
                return compare_header.first.new_lowercase() == name.new_lowercase();
            });
}


auto fetch::helpers::header_internals::get(
        const header_name_t& name,
        const header_list_t& list)
        -> ext::optional<header_value_t>
{
    return contains_header(name, list)
            ? ext::optional((ext::string) list.filter([name](const header_name_t& header_name) {return header_name.new_lowercase() == name.new_lowercase();})
                    .transform([](const header_t& header) {return header.second;})
                    .join((char) 0x2C))
            : std::nullopt;
}


auto fetch::helpers::header_internals::get_decode_split(
        const header_name_t& name,
        const header_list_t& list)
        -> ext::vector<header_value_t>
{
    // get the value from the list, if empty then return the nullopt
    auto initial_value = get(name, list);
    if (not initial_value.has_value())
        return std::nullopt;

    // get the input, and he iterator pointing to the beginning of the input
    auto input = *initial_value;
    auto position = input.begin();

    // create the values list and the current value object
    ext::vector<header_value_t> values;
    header_value_t value;

    // iterate until the end of the string is reached
    while (position != input.end())
    {
        // get the next substring point (where the next speech mark or comma is), and substring input accordingly
        auto next = input.find_first_that_is('\"', ',');
        value    += input = input.substring(position + 1, next); // TODO : brackets needed?
        position += std::distance(position, next);

        // check that the end of the string hasn't been reached yet
        if (position != input.end())
        {
            // handle the speech mark
            if (*position == '\"')
            {
                // TODO : collect HTTP quote code points
                if (position != input.end()) continue;
            }

            // handle anything else (must be comma, otherwise the syntax is incorrect)
            else
            {
                assert(*position == ',');
                position += 1;
            }
        }

        // trim spaces and tabs from either end of the value, append it to the values, and reset the value
        value.trim(' ', '\t');
        values.append(value);
        value = "";
    }

    return values;
}


auto fetch::helpers::header_internals::append(
        const header_t& header,
        header_list_t& list)
        -> void
{
    if (contains_header(header.first, list)); // TODO ?

    // append the header name-value pair to the list
    list.append(header);
}


auto fetch::helpers::header_internals::delete_(
        const header_name_t& name,
        header_list_t list)
        -> void
{
    // remove all headers that have the same name
    list.remove_if([name](const header_t& header) {return header.first == name;}, true);
}


auto fetch::helpers::header_internals::set(
        const header_t& header,
        header_list_t& list)
        -> void
{
    if (contains_header(header.first, list))
    {
        header_list_t matching_headers;
        matching_headers = list
                .filter([header](const header_t& compare_header) {return header.first == compare_header.first;})
                .slice(matching_headers.begin() + 1, matching_headers.end());

        list.remove_if([matching_headers](const header_t& remove_header) {return matching_headers.contains(remove_header);});
    }
}
