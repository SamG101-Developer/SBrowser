#pragma once
#ifndef SBROWSER_TEXT_TRACK_CUE_HPP
#define SBROWSER_TEXT_TRACK_CUE_HPP

#include <dom/nodes/event_target.hpp>
#include <ext/html_property.hpp>

namespace html::media {class text_track;}
namespace html::media {class text_track_cue;}


class html::media::text_track_cue : public dom::nodes::event_target
{
public constructors:
    text_track_cue();

public js_properties:
    ext::html_property<text_track*> track;
    ext::html_property<ext::string> id;
    ext::html_property<double> start_time;
    ext::html_property<double> end_time;
    ext::html_property<bool> pause_on_exit;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

public cpp_properties:
    bool m_active_flag;
    int m_display_state; // TODO : check type is correct (int)
};


#endif //SBROWSER_TEXT_TRACK_CUE_HPP
