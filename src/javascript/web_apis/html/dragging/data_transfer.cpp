#include "data_transfer.hpp"

#include <dom/nodes/element.hpp>

#include <html/elements/html_image_element.hpp>
#include <html/helpers/canvas_internals.hpp>
#include <html/helpers/dragging_internals.hpp>

#include <QtCore/QRect>
#include <QtCore/QSize>
#include <QtGui/QPixmap>
#include <QtGui/QRegion>
#include <QtWidgets/QWidget>

#include <render/painting/image.hpp>


html::dragging::data_transfer::data_transfer()
{
    // constrain the property values
    effect_allowed.constrain_values({"none", "copy", "copyLink", "copyMove", "all", "link", "linkMove", "move", "uninitialized"});

    // set the property values
    drop_effect    = "none";
    effect_allowed = "none";

    // set the attribute values
    m_drag_data_store = new internal::drag_data_store{};
    m_drag_data_store->drag_data_store_mode = helpers::dragging_internals::drag_data_store_mode_t::READ_WRITE;
}


auto html::dragging::data_transfer::set_drag_image(
        dom::nodes::element* image,
        const long x,
        const long y) -> void
{
    if (not m_drag_data_store)
        return;

    if (m_drag_data_store->drag_data_store_mode == helpers::dragging_internals::drag_data_store_mode_t::READ_WRITE)
        return;

    QPixmap pixmap;
    if (auto* image_element = dynamic_cast<elements::html_image_element*>(image))
        pixmap = image_element->qt()->pixmap();
    else
    {
        QRect rectangle {0, 0, image->client_width, image->client_height};
        pixmap = QPixmap{rectangle.size()};
        image->qt()->render(&pixmap, QPoint{}, QRegion{rectangle});
    }

    // TODO : m_drag_data_store->drag_data_store_bitmap = &(canvas::image_bitmap)render::painting::image{pixmap};

    m_drag_data_store->drag_data_store_hot_spot_coordinate = new internal::point{.x=static_cast<double>(x), .y=static_cast<double>(y)};
}
