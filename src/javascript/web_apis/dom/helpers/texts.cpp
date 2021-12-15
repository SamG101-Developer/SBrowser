#include "texts.hpp"

#include <javascript/environment/realms.hpp>

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_observers.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/nodes/character_data.hpp>


void
dom::helpers::texts::replace_data(
        nodes::character_data* text_node,
        unsigned long offset,
        unsigned long count,
        ext::cstring& data) {

    const unsigned long length = trees::length(text_node);
    ext::string current_data = text_node->data;

    exceptions::throw_v8_exception(
            "offset must be <= length of the character_data node",
            INDEX_SIZE_ERR,
            [offset, count] -> bool {return offset > count;});

    mutation_observers::queue_mutation_record("characterData", text_node, "", "", text_node->data, {}, {}, nullptr, nullptr);
    count = std::min(count, length - offset);
    text_node->data = current_data.replace(offset, count, data);

    ext::vector<ranges::range*> live_ranges = javascript::realms::surrounding_agent().get<ext::vector<ranges::range*>("live_ranges");

    live_ranges.filter([text_node, offset, count](auto* range) -> bool {return range->start_container == text_node and})
}
