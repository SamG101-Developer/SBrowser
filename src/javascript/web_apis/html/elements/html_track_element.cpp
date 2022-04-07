#include "html_track_element.hpp"

#include <dom/helpers/trees.hpp>
#include <html/elements/html_media_element.hpp>


html::elements::html_track_element::html_track_element()
        : html_element()
{
    // constrain the attribute values
    kind.constrain_values({"subtitles", "captions", "descriptions", "chapters", "metadata"});
}


auto html::elements::html_track_element::set_parent_node(
        dom::nodes::node* val)
        -> void
{
    // call the default behaviour
    html_element::set_parent_node(val);

    // remove this element's track from any list of pending tracks that it is in
    dom::helpers::trees::descendants(dom::helpers::trees::root(this))
            .cast_all<html_media_element*>()
            .filter([this](html_media_element* element) {return element->m_pending_text_tracks.contains(track);})
            .for_each([this](html_media_element* element) {element->m_pending_text_tracks.remove(track, true);});
}
