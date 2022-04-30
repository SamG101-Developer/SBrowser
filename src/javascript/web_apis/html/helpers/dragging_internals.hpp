#pragma once
#ifndef SBROWSER_DRAGGING_INTERNALS_HPP
#define SBROWSER_DRAGGING_INTERNALS_HPP

namespace html::helpers {struct dragging_internals;}

#include <ext/string.hpp>

namespace html::canvas::paint {class image_bitmap;}
namespace html::dragging {class data_transfer_item;}
namespace html::internal {struct drag_data_store;}
namespace html::internal {struct point;}


struct html::helpers::dragging_internals
{
    using drag_store_item_t = struct
    {
        dragging::data_transfer_item* drag_data_item_kind;
        ext::string drag_data_item_type_string;
        ext::string actual_data;
    };

    enum class drag_data_store_mode_t
    {
        READ_WRITE,
        READ_ONLY,
        PROTECTED
    };
};


struct html::internal::drag_data_store
{
    ext::vector<helpers::dragging_internals::drag_store_item_t*> drag_data_store_item_list;
    ext::string drag_data_store_default_feedback;
    ext::string drag_data_store_allowed_effects_state = "uninitialized";
    helpers::dragging_internals::drag_data_store_mode_t drag_data_store_mode = helpers::dragging_internals::drag_data_store_mode_t::PROTECTED;
    canvas::paint::image_bitmap* drag_data_store_bitmap = nullptr;
    point* drag_data_store_hot_spot_coordinate;
};


#endif //SBROWSER_DRAGGING_INTERNALS_HPP
