#pragma once
#ifndef SBROWSER_IMAGE_INTERNALS_HPP
#define SBROWSER_IMAGE_INTERNALS_HPP

#include <ext/map.hpp>
#include <ext/string.hpp>
#include <ext/type_traits.hpp>

namespace html::canvas::paint {class image_data;}
namespace html::elements {class html_image_element;}
namespace html::internal {struct image_dimensions;}
namespace html::internal {enum class image_request_state;}
namespace html::internal {enum class image_decode_result;}
namespace html::internal {struct image_request;}
namespace html::helpers {struct image_internals;}


struct html::helpers::image_internals
{
    static auto is_image_available(
            const internal::image_request* request)
    -> bool;

    static auto is_image_fully_decodable(
            const internal::image_request* request)
    -> bool;

    static auto density_corrected_intrinsic_size(
            elements::html_image_element* element)
    -> internal::image_dimensions*;

    static auto update_image_data(
            elements::html_image_element* element,
            bool restart_animations_flag)
    -> void;

    static auto uses_srcset_or_picture(
            elements::html_image_element* element
    ) -> bool;

    static auto abort_image_request(
            internal::image_request* request)
    -> void;

    static auto upgrade_pending_request_to_current_request(
            elements::html_image_element* element)
    -> void;

    static auto prepare_image_for_presentation(
            internal::image_request* request)
    -> void;

    static auto select_an_image_source(
            // TODO : mixin for HTMLLinkElement & HTMLImageElement for the src_set attribute
    ) -> void;

    static auto initialize_image(
            html::canvas::paint::image_data* image_data,
            ulong width,
            ulong height,
            const ext::string_any_map_t& settings,
            ext::uint8_array* source = nullptr,
            const ext::string& default_color_space = "")
            -> void;
};


struct html::internal::image_dimensions
{
    float height;
    float width;
};


enum class html::internal::image_request_state
{
    UNAVAILABLE,
    PARTIALLY_AVAILABLE,
    COMPLETELY_AVAILABLE,
    BROKEN,
};


enum class html::internal::image_decode_result
{
    SKIP,
    FAIL,
    SUCCESS,
};


struct html::internal::image_request
{
    image_request_state state = image_request_state::UNAVAILABLE;
    ext::string current_url = "";
    ext::vector<std::byte> image_data {};
    float current_pixel_density = 1.0;
    internal::image_dimensions* preferred_density_corrected_dimensions = nullptr;
};


#endif //SBROWSER_IMAGE_INTERNALS_HPP
