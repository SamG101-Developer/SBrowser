#include "html_image_element.hpp"

#include <thread>

#include <dom/helpers/mutation_observers.hpp>
#include <dom/helpers/trees.hpp>
#include <dom/nodes/document.hpp>
#include <dom/other/dom_exception.hpp>

#include <html/helpers/custom_html_elements.hpp>
#include <html/helpers/document_internals.hpp>
#include <html/helpers/images.hpp>


html::elements::html_image_element::html_image_element()
        : html_element{}
        , mixins::lazy_loading{}
{
    // set the attributes
    m_dimension_attribute_src = this;

    // set the custom accessors
    width.getter = [this] {return get_width();};
    height.getter = [this] {return get_height();};
    natural_width.getter = [this] {return get_natural_width();};
    natural_height.getter = [this] {return get_natural_height();};
    complete.getter = [this] {return get_complete();};

    alt.setter = [this](auto&& PH1) {set_alt(std::forward<decltype(PH1)>(PH1));};
    src.setter = [this](auto&& PH1) {set_src(std::forward<decltype(PH1)>(PH1));};
    width.setter = [this](auto&& PH1) {set_width(std::forward<decltype(PH1)>(PH1));};
    height.setter = [this](auto&& PH1) {set_height(std::forward<decltype(PH1)>(PH1));};

    // attach the qt functions
    alt.attach_qt_updater(&QLabel::setText, this);

    // create the widget representation
    auto widget = QPointer<QLabel>{};
    widget->setPixmap(QPixmap{});
    m_rendered_widget = widget;

    // initialize html constructor with boilerplate code
    HTML_CONSTRUCTOR
}


auto html::elements::html_image_element::decode()
        -> webidl::types::promise<void>
{
    webidl::types::promise<void> promise;
    dom::helpers::mutation_observers::queue_microtask([this, &promise] {
        // the exception that will be thrown if there is a decoding error
        dom::other::dom_exception encoding_exception {"Image encoding / decoding error", ENCODING_ERR};

        // if the document is active or the current request is broken, then reject the promise with the decoding error
        if (helpers::document_internals::is_active_document(owner_document) or m_current_request->state == internal::image_request_state::BROKEN)
            promise.reject(encoding_exception);
        else
        {
            // run the next steps in an auto-joining thread (includes a while loop, so run in parallel)
            auto thread = std::jthread([this, &promise, encoding_exception] {
                // save the previous request into a new pointer
                auto previous_request = new internal::image_request{*m_current_request};

                // loop until a breaking condition is met
                while (true)
                {
                    // if the document becomes active, the image data changes, or the current request becomes broke,
                    // then reject the promise
                    if (not helpers::document_internals::is_active_document(owner_document)
                            or m_current_request->image_data != previous_request->image_data
                            or m_current_request->state == internal::image_request_state::BROKEN)
                    {
                        // reject the promise with the decoding error and break from the loop
                        promise.reject(encoding_exception);
                        break;
                    }

                    // if the current request state becomes completely available, then decode the image and either
                    // reject or resolve the promise depending on the decoding result
                    if (m_current_request->state == internal::image_request_state::COMPLETELY_AVAILABLE)
                    {
                        // get the result from decoding the image and switch on the result
                        auto result = helpers::images::decode_image(this);
                        switch(result)
                        {
                            // resolve the promise if the image is already decoded (so skip)
                            case internal::image_decode_result::SKIP:
                                promise.resolve();
                                break;

                            // reject the promise if the image failed to decode
                            case internal::image_decode_result::FAIL:
                                promise.reject(encoding_exception);
                                break;

                            // resolve the promise if the image decoded successfully
                            case internal::image_decode_result::SUCCESS:
                                promise.resolve();
                                break;
                        }
                        // break from the while loop
                        break;
                    }

                    // set the previous request to a copy of the current request before the current request is checked
                    // again
                    previous_request = nullptr;
                    delete previous_request;
                    previous_request = new internal::image_request{*m_current_request};
                }
            });
        }
    });
}


auto html::elements::html_image_element::get_width() const
        -> unsigned long
{
    // return the width of the pixmap
    return render()->isVisible() ? render()->pixmap().width() : html::helpers::images::density_corrected_intrinsic_size(this).width;
}


auto html::elements::html_image_element::get_height() const
        -> unsigned long
{
    // return the height of the pixmap
    return render()->isVisible() ? render()->pixmap().height() : html::helpers::images::density_corrected_intrinsic_size(this).height;
}


auto html::elements::html_image_element::get_natural_width() const
        -> unsigned long
{
    // return the density-corrected intrinsic width
    return css::css_images::helpers::images::has_instrinsic_dimensions(this) ? html::helpers::images::density_corrected_intrinsic_size(this).width : 0;
}


auto html::elements::html_image_element::get_natural_height() const
        -> unsigned long
{
    // return the density-corrected intrinsic height
    return css::css_images::helpers::images::has_instrinsic_dimensions(this) ? html::helpers::images::density_corrected_intrinsic_size(this).height : 0;
}


auto html::elements::html_image_element::get_complete()
        -> bool
{
    // return if the image is complete or not
    return (src->empty() and srcset->empty())
            or (m_current_request->state == internal::image_request_state::COMPLETELY_AVAILABLE and not m_pending_request)
            or (m_current_request->state == internal::image_request_state::BROKEN               and not m_pending_request)
}


auto html::elements::html_image_element::set_alt(
        const ext::string& val)
        -> void
{
    alt << val;

    // if {src: set, image: valid, alt: empty (doesn't matter)}, then do nothing (render the image)
    if (src and not render()->pixmap().isNull())
        ;

        // if {src: set, image: NOT valid, alt NOT empty}, then set the label to the alt text
    else if (src and render()->pixmap().isNull() and not alt->empty())
        render()->setText(alt);

        // if {src: set, image: NOT valid, alt empty}, then set the label to the title etc
    else if (src and render()->pixmap().isNull() and alt->empty())
    {
        // set the displayed text to te title if it exists
        if (not title->empty())
            render()->setText(title);

            // otherwise, set the displayed text to the first related figcaption's text content
        else
        {
            // get the ancestor figure element
            auto* ancestor_figure = dom::helpers::trees::ancestors(this)
                    .cast_all<html_element*>()
                    .filter([](html_element* element) {return element->local_name == "figure";}).front();

            // get the figure's figcaption descendant
            auto* related_figcaption = ancestor_figure->children
                    ->cast_all<html_element*>()
                    .filter([](html_element* element) {return element->local_name == "figcaption";}).front();

            // set the text to the descendant text content of the figcaption
            render()->setText(dom::helpers::trees::descendant_text_content(related_figcaption));
        }
    }

        // if {src: NOT set, image: NOT valid (implied), alt: empty}, then the image is empty, don't render
    else
        render()->clear();
}


auto html::elements::html_image_element::set_src(
        const ext::string& val)
        -> void
{
    // TODO : fetch resource (using cross_origin, referrer_policy, decoding, loading)
}


auto html::elements::html_image_element::set_width(
        const unsigned long val)
        -> void
{
    // set the width of the image
    width << val;
    render()->pixmap().scaledToWidth(width);
}


auto html::elements::html_image_element::set_height(
        const unsigned long val)
        -> void
{
    // set the height for the image
    height << val;
    render()->pixmap().scaledToHeight(height);
}


auto html::elements::html_image_element::render() const
        -> QLabel*
{
    return qobject_cast<QLabel*>(m_rendered_widget);
}


auto html::elements::html_image_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    v8pp::class_<html_image_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .inherit<mixins::lazy_loading>()
            .function("decode", &html_image_element::decode)
            .var("alt", &html_image_element::alt)
            .var("src", &html_image_element::src)
            .var("srcset", &html_image_element::srcset)
            .var("sizes", &html_image_element::sizes)
            .var("cross_origin", &html_image_element::cross_origin)
            .var("use_map", &html_image_element::use_map)
            .var("referrer_policy", &html_image_element::referrer_policy)
            .var("decoding", &html_image_element::decoding)
            .var("current_src", &html_image_element::current_src)
            .var("width", &html_image_element::width)
            .var("height", &html_image_element::height)
            .var("natural_width", &html_image_element::natural_width)
            .var("natural_height", &html_image_element::natural_height)
            .var("is_map", &html_image_element::is_map)
            .var("complete", &html_image_element::complete)
            .auto_wrap_objects();
}
