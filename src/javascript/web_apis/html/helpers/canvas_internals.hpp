#pragma once
#ifndef SBROWSER_CANVAS_INTERNALS_HPP
#define SBROWSER_CANVAS_INTERNALS_HPP

#include <optional>
#include <ext/map.hpp>
#include <ext/type_traits.hpp>

#include <render/painting/painter_path.hpp>

#include <QtCore/QPointF>
#include <QtGui/QPainterPath>

namespace html::canvas {class canvas_rendering_context_2d;}
namespace html::canvas {class image_bitmap_rendering_context;}
namespace html::canvas::mixins {template <typename T> class canvas_text_drawing_styles;}
namespace html::canvas::mixins {template <typename T> class canvas_path;}
namespace html::canvas::mixins {template <typename T> class canvas_draw_path;}
namespace html::canvas::paint {class path_2d;}
namespace html::canvas::paint {class image_bitmap;}
namespace html::elements {class html_canvas_element;}
namespace html::helpers {struct canvas_internals;}
namespace html::internal {struct drawing_state;}
namespace html::internal {struct point;}
namespace html::internal {struct subpath;}
namespace html::internal {struct glyph;}


struct html::helpers::canvas_internals
{
    enum class alignment {LEFT, CENTER, RIGHT};

    static auto _2d_context_creation_algorithm(
            elements::html_canvas_element* canvas_element,
            const ext::string_any_map& options)
            -> canvas::canvas_rendering_context_2d*;

    static auto offscreen_2d_context_creation_algorithm(
            // TODO
            )

    static auto set_bitmap_dimensions(
            elements::html_canvas_element* canvas_element,
            ulong width,
            ulong height)
            -> void;

    static auto reset_canvas_context_to_default_state(
            elements::html_canvas_element* canvas_element)
            -> void;

    template <typename T>
    static auto text_preparation_algorithm(
            const ext::string& text,
            canvas::mixins::canvas_text_drawing_styles<T>* target,
            std::optional<int> max_width) -> ext::vector<internal::glyph>;

    template <typename T>
    static auto ensure_there_is_subpath(
            const typename canvas::mixins::canvas_path<T>::point_t& point,
            const canvas::mixins::canvas_path<T>* path)
            -> void;

    static auto are_collinear(
            const internal::point& p1,
            const internal::point& p2,
            const internal::point& p3)
            -> bool;

    static auto set_image_bitmap_rendering_contexts_output_bitmap(
            canvas::image_bitmap_rendering_context* context,
            canvas::paint::image_bitmap* bitmap = nullptr)
            -> void;
};


struct html::internal::drawing_state {};

struct html::internal::point
{
    double x;
    double y;

    operator QPointF() const {return {x, y};}
    auto operator==(const point& other) const {return x == other.x and y == other.y;}
};


struct html::internal::subpath
{
    ext::vector<point> segments;
    bool is_subpath_closed = false;
    render::painting::painter_path qt_painter_subpath;
};


struct html::internal::glyph
{
    double x;
    double y;
    ext::string text;
};


#endif //SBROWSER_CANVAS_INTERNALS_HPP
