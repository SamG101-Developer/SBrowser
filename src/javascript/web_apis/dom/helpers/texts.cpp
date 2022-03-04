#include "texts.hpp"

#include <ext/iterables/string.hpp>

#include <javascript/environment/realms.hpp>

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/mutation_observers.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/nodes/text.hpp>

#include <dom/ranges/range.hpp>


auto dom::helpers::texts::replace_data(
        nodes::character_data* const text_node,
        const unsigned long offset,
        unsigned long count,
        const ext::string& data)
        -> void
{
    // get the text node's text and the length of the text node
    const auto& current_data = text_node->data;
    const auto length = trees::length(text_node);

    // if teh offset > count, then throw an index size error
    exceptions::throw_v8_exception(
            "offset must be <= length of the character_data node",
            INDEX_SIZE_ERR,
            [offset, count] {return offset > count;});

    // queue a mutation record for the text node, as the data is about to be replaced
    mutation_observers::queue_mutation_record("characterData", text_node, "", "", text_node->data, {}, {}, nullptr, nullptr);

    // cap the count at the max length - offset, and replace the data in the text node at the correct offset and count
    count = std::min(count, length - offset);
    text_node->data = current_data->replace(offset, count, data);

    // get the live ranges from the javascript environment
    auto& live_ranges = javascript::realms::surrounding_agent().get<ext::vector<ranges::range*>&>("live_ranges");

    // live ranges whose start container is the text node and start offset is contained in the start and end text
    // offsets, change the start offset to the start offset of text replacement
    live_ranges
            .filter([text_node](auto* range) {return range->start_container == text_node;})
            .filter([count, offset](auto* range) {return range->start_offset > offset and range->start_offset <= offset + count;})
            .for_each([offset](auto* range) {range->start_offset = offset;});

    // live ranges whose end container is the text node and end offset is contained in the start and end text offsets,
    // change the end offset to the start offset of text replacement
    live_ranges
            .filter([text_node](auto* range) {return range->end_container == text_node;})
            .filter([count, offset](auto* range) {return range->end_offset > offset and range->end_offset <= offset + count;})
            .for_each([offset](auto* range) {range->end_offset = offset;});

    // live ranges whose start container is the text node and start offset is after end text offset, increment the start
    // offset by the [length of the data - the count]
    live_ranges
            .filter([text_node](auto* range) {return range->start_container == text_node;})
            .filter([count, offset](auto* range) {return range->start_offset > offset + count;})
            .for_each([count, data](auto* range) {range->start_offset += data.length() - count;});

    // live ranges whose end container is the text node and end offset is after end text offset, increment the end
    // offset by the [length of the data - the count]
    live_ranges
            .filter([text_node](auto* range) {return range->end_container == text_node;})
            .filter([count, offset](auto* range) {return range->end_offset > offset + count;})
            .for_each([count, data](auto* range) {range->end_offset += data.length() - count;});
}


auto dom::helpers::texts::split(
        nodes::text* const text_node,
        const unsigned long offset)
        -> dom::nodes::text*
{
    const auto length = trees::length(text_node);
    const auto count = length - offset;
    const auto new_data = substring_data(text_node, offset, count);

    exceptions::throw_v8_exception(
            "offset must be <= length of the text node",
            INDEX_SIZE_ERR,
            [offset, length] {return offset > length;});

    auto* const new_text_node = new nodes::text{new_data};
    new_text_node->owner_document = text_node->owner_document;

    if (text_node->parent) {
        mutation_algorithms::insert(new_text_node, text_node->parent, new_text_node->next_sibling);
        auto& live_ranges = javascript::realms::surrounding_agent().get<ext::vector<ranges::range*>&>("live_ranges");

        live_ranges
                .filter([text_node](auto* range) {return range->start_container == text_node;})
                .filter([offset](auto* range) {return range->start_offset > offset;})
                .for_each([new_text_node, offset](auto* range) {range->start_container = new_text_node; range->start_offset = offset;});

        live_ranges
                .filter([text_node](auto* range) {return range->end_container == text_node;})
                .filter([offset](auto* range) {return range->end_offset > offset;})
                .for_each([new_text_node, offset](auto* range) {range->end_container = new_text_node; range->start_offset = offset;});

        live_ranges
                .filter([text_node](auto* range) {return range->start_container == text_node->parent;})
                .filter([text_node](auto* range) {return range->start_offset == trees::index(text_node) + 1;})
                .for_each([new_text_node, offset](auto* range) {range->start_container = new_text_node; range->start_offset = offset;});

        live_ranges
                .filter([text_node](auto* range) {return range->end_container == text_node->parent;})
                .filter([text_node](auto* range) {return range->end_offset == trees::index(text_node) + 1;})
                .for_each([new_text_node, offset](auto* range) {range->end_container = new_text_node; range->start_offset = offset;});
    }

    replace_data(text_node, offset, count, "");
    return new_text_node;
}


auto dom::helpers::texts::substring_data(
        const nodes::character_data* const text_node,
        const unsigned long offset,
        unsigned long count) -> ext::string
{
    const auto length = trees::length(text_node);
    exceptions::throw_v8_exception(
            "offset must be <= length of the text node",
            INDEX_SIZE_ERR,
            [offset, length] {return offset > length;});

    count = std::min(count, length - offset);
    ext::cstring current_data = text_node->data;
    return current_data.substring(offset, count);
}
