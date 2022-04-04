#include "html_track_element.hpp"


html::elements::html_track_element::html_track_element()
        : html_element()
{
    // constrain the attribute values
    kind.constrain_values({"subtitles", "captions", "descriptions", "chapters", "metadata"});
}
