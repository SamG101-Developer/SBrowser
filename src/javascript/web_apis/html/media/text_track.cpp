#include "text_track.hpp"


html::media::text_track::text_track()
        : abstract_track()
{
    // constrain the attribute values
    kind.constrain_values({"subtitles", "descriptions", "chapters", "metadata"});
    mode.constrain_values({"disabled", "hidden", "showing"});

    // set the attributes
    m_readiness_state = helpers::media_internals::track_readiness_state::NOT_LOADED;
}
