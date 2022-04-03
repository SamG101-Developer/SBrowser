#include "html_track_element.hpp"


html::elements::html_track_element::html_track_element() = default;


auto html::elements::html_track_element::set_kind(
        const ext::string& val)
        -> void
{
    ext::vector<ext::string> allowed_values {
            "subtitles",
            "captions",
            "descriptions",
            "chapters",
            "metadata"
    };

    if (allowed_values.contains(val))
        kind << val;
}
