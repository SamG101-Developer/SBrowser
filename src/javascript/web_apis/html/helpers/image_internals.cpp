#include "image_internals.hpp"

#include <thread>

#include <dom/helpers/mutation_observers.hpp>

#include <html/canvas/paint/image_data.hpp>
#include <html/elements/html_image_element.hpp>
#include <html/elements/html_picture_element.hpp>

#include <html/helpers/document_internals.hpp>


auto html::helpers::image_internals::is_image_available(
        const internal::image_request* request)
        -> bool
{
    // an image request is available if it is partially or completely available
    return request->state == internal::image_request_state::PARTIALLY_AVAILABLE
            or request->state == internal::image_request_state::COMPLETELY_AVAILABLE;
}


auto html::helpers::image_internals::is_image_fully_decodable(
        const internal::image_request* request)
        -> bool
{
    // an image is fully decodable if it is completely available
    return request->state == internal::image_request_state::COMPLETELY_AVAILABLE;
}


auto html::helpers::image_internals::density_corrected_intrinsic_size(
        elements::html_image_element* element)
        -> internal::image_dimensions*
{
    //
    auto* dim = element->m_current_request->preferred_density_corrected_dimensions;
    // TODO : dim = dim ? dim : intrinsic dimensions of the element

    dim->width  /= element->m_current_request->current_pixel_density;
    dim->height /= element->m_current_request->current_pixel_density;
    return dim;
}


auto html::helpers::image_internals::update_image_data(
        elements::html_image_element* element,
        const bool restart_animations_flag)
        -> void
{
    if (not document_internals::is_active_document(element->owner_document))
    {
        auto thread = std::jthread([element] {
            while (not document_internals::is_active_document(element->owner_document));
            // TODO : check if this method has been ran again during this thread execution
            dom::helpers::mutation_observers::queue_microtask(/* TODO : continue this algorithm -> how to put into lambda? */);
        });
    }

    // TODO : if image support revoked / no image support handling here

    auto src_check = uses_srcset_or_picture(element) and not element->src->empty();
    auto selected_pixel_density = src_check ? 1.0 : 0.0;
    ext::string selected_source = src_check ? element->src : "";

    element->m_last_selected_source = selected_source;
    // TODO : fetching and requests etc

}


auto html::helpers::image_internals::uses_srcset_or_picture(
        elements::html_image_element* element)
        -> bool
{
    // check if the element has a src set or is the child of a HTMLPictureElement
    return not element->srcset->empty() or ext::property_dynamic_cast<elements::html_picture_element*>(element->parent_element);
}


auto html::helpers::image_internals::abort_image_request(
        internal::image_request* request)
        -> void
{
    request->image_data.clear();
    // TODO : abort fetching algorithms running
}


auto html::helpers::image_internals::upgrade_pending_request_to_current_request(
        elements::html_image_element* element)
        -> void
{
    element->m_current_request = element->m_pending_request;
    element->m_pending_request = nullptr;
}


auto html::helpers::image_internals::initialize_image(
        html::canvas::paint::image_data* image_data,
        const ulong width,
        const ulong height,
        const ext::string_any_map_t& settings,
        ext::uint8_array* source,
        const ext::string& default_color_space)
        -> void
{
    image_data->data = source->empty() ? new ext::uint8_array{} : source;
    image_data->data->reserve(width * height);

    image_data->width = width;
    image_data->height = height;

    if (not settings.empty() and settings.has_key("colorSpace"))
        image_data->color_space = settings.at("colorSpace");
    else if (not default_color_space.empty())
        image_data->color_space = default_color_space;
    else
        image_data->color_space = "srgb";
}
