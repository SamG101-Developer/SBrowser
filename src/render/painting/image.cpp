#include "image.hpp"

#include <QtGui/QPixmap>


auto render::painting::image::reset()
        -> void
{
    // set the image to a transparent black pixmap of the right size
    QPixmap pixmap{m_image.width(), m_image.height()};
    pixmap.fill(QColor{"#ff000000"});
    m_image = pixmap.toImage();
}
