#include "html_video_element.hpp"


html::elements::html_video_element::html_video_element()
        : html_media_element()
{
    // attach the qt functions
    width.attach_qt_updater(&QWidget::setFixedWidth, this);
    height.attach_qt_updater(&QWidget::setFixedHeight, this);

    // create the widget representation
    m_rendered_widget = QPointer<QVideoWidget>{};
}


auto html::elements::html_video_element::qt() const
        -> QVideoWidget*
{
    return qobject_cast<QVideoWidget*>(m_rendered_widget);
}


auto html::elements::html_video_element::get_video_width() const
        -> ulong
{
    // get the width of the video
    return ready_state == HAVE_NOTHING ? 0 : qt()->width();
}


auto html::elements::html_video_element::get_video_height() const
        -> ulong
{
    // get the height of the video
    return ready_state == HAVE_NOTHING ? 0 : qt()->height();
}


auto html::elements::html_video_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_video_element>{isolate}
            .ctor<>()
            .inherit<html_media_element>()
            .var("width", &html_video_element::width)
            .var("height", &html_video_element::height)
            .var("video_width", &html_video_element::video_width)
            .var("video_height", &html_video_element::video_height)
            .var("poster", &html_video_element::poster)
            .var("plays_inline", &html_video_element::plays_inline)
            .auto_wrap_objects();
}
