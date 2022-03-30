#pragma once
#ifndef SBROWSER_IMAGES_HPP
#define SBROWSER_IMAGES_HPP

#include <ext/string.hpp>

namespace html::elements {class html_image_element;}
namespace html::internal {struct image_dimensions;}
namespace html::internal {enum class image_request_state;}
namespace html::internal {enum class image_decode_result;}
namespace html::internal {struct image_request;}
namespace html::helpers {struct images;}


struct html::internal::image_dimensions
{
    int height;
    int width;
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
};


struct html::helpers::images
{
    static bool is_image_available(const internal::image_request* const request);
    static internal::image_dimensions density_corrected_intrinsic_size(html::elements::html_image_element* element);
};


#endif //SBROWSER_IMAGES_HPP
