#ifndef SBROWSER_EXPOSE_CPP_TO_JS_HPP
#define SBROWSER_EXPOSE_CPP_TO_JS_HPP

#include <javascript/environment/modules.hpp>

#include <dom/aborting/abort_controller.hpp>
#include <dom/aborting/abort_signal.hpp>

#include <dom/events/event.hpp>
#include <dom/events/custom_event.hpp>

#include <dom/iterators/abstract_iterator.hpp>
#include <dom/iterators/node_filter.hpp>
#include <dom/iterators/node_iterator.hpp>
#include <dom/iterators/tree_walker.hpp>

#include <dom/mixins/child_node.hpp>
#include <dom/mixins/document_or_element_node.hpp>
#include <dom/mixins/document_or_shadow_root.hpp>
#include <dom/mixins/non_document_type_child_node.hpp>
#include <dom/mixins/non_element_parent_node.hpp>
#include <dom/mixins/parent_node.hpp>
#include <dom/mixins/slottable.hpp>

#include <dom/mutations/mutation_observer.hpp>
#include <dom/mutations/mutation_record.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/cdata_section.hpp>
#include <dom/nodes/character_data.hpp>
#include <dom/nodes/comment.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/event_target.hpp>
#include <dom/nodes/node.hpp>
#include <dom/nodes/processing_instruction.hpp>
#include <dom/nodes/shadow_root.hpp>
#include <dom/nodes/text.hpp>
#include <dom/nodes/window.hpp>
#include <dom/nodes/window_proxy.hpp>
#include <dom/nodes/xml_document.hpp>

#include <dom/other/dom_exception.hpp>
#include <dom/other/dom_implementation.hpp>
#include <dom/other/xslt_processor.hpp>
#include <dom/other/xslt_processor.hpp>

#include <dom/ranges/abstract_range.hpp>
#include <dom/ranges/range.hpp>
#include <dom/ranges/static_range.hpp>

#include <dom/xpath/xpath_evaluator.hpp>
#include <dom/xpath/xpath_expression.hpp>
#include <dom/xpath/xpath_result.hpp>


#include <html/elements/html_anchor_element.hpp>
#include <html/elements/html_area_element.hpp>
#include <html/elements/html_audio_element.hpp>
#include <html/elements/html_base_element.hpp>
#include <html/elements/html_body_element.hpp>
#include <html/elements/html_br_element.hpp>
#include <html/elements/html_button_element.hpp>
#include <html/elements/html_canvas_element.hpp>
#include <html/elements/html_data_element.hpp>
#include <html/elements/html_datalist_element.hpp>
#include <html/elements/html_details_element.hpp>
#include <html/elements/html_dialog_element.hpp>
#include <html/elements/html_div_element.hpp>
#include <html/elements/html_dlist_element.hpp>
#include <html/elements/html_element.hpp>
#include <html/elements/html_form_element.hpp>
#include <html/elements/html_head_element.hpp>
#include <html/elements/html_heading_element.hpp>
#include <html/elements/html_hr_element.hpp>
#include <html/elements/html_html_element.hpp>
#include <html/elements/html_iframe_element.hpp>
#include <html/elements/html_image_element.hpp>
#include <html/elements/html_label_element.hpp>
#include <html/elements/html_li_element.hpp>
#include <html/elements/html_link_element.hpp>
#include <html/elements/html_map_element.hpp>
#include <html/elements/html_media_element.hpp>
#include <html/elements/html_meta_element.hpp>
#include <html/elements/html_menu_element.hpp>
#include <html/elements/html_mod_element.hpp>
#include <html/elements/html_olist_element.hpp>
#include <html/elements/html_paragraph_element.hpp>
#include <html/elements/html_picture_element.hpp>
#include <html/elements/html_pre_element.hpp>
#include <html/elements/html_quote_element.hpp>
#include <html/elements/html_slot_element.hpp>
#include <html/elements/html_source_element.hpp>
#include <html/elements/html_span_element.hpp>
#include <html/elements/html_style_element.hpp>
#include <html/elements/html_table_caption_element.hpp>
#include <html/elements/html_table_cell_element.hpp>
#include <html/elements/html_table_col_element.hpp>
#include <html/elements/html_table_element.hpp>
#include <html/elements/html_table_row_element.hpp>
#include <html/elements/html_table_section_element.hpp>
#include <html/elements/html_title_element.hpp>
#include <html/elements/html_time_element.hpp>
#include <html/elements/html_track_element.hpp>
#include <html/elements/html_ulist_element.hpp>
#include <html/elements/html_unknown_element.hpp>
#include <html/elements/html_video_element.hpp>

#include <html/events/track_event.hpp>

#include <html/media/audio_track.hpp>
#include <html/media/text_track.hpp>
#include <html/media/text_track_cue.hpp>
#include <html/media/video_track.hpp>
#include <html/media/time_ranges.hpp>

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/module.hpp>


namespace javascript::interop::expose_cpp_to_js
{
    template <typename T> auto object_to_v8(v8::Isolate* isolate) requires std::is_base_of_v<dom_object, T>;
    auto expose(v8::Isolate* isolate, v8::Persistent<v8::Context>& persistent_context, javascript::environment::modules::module_type module_type) -> void;
}


template <typename T>
auto javascript::interop::expose_cpp_to_js::object_to_v8(v8::Isolate* isolate) requires std::is_base_of_v<dom_object, T>
{
    // create a temporary instance of the object, return the v8 conversion, and map it to the correct type. an instance
    // has to be made, as the conversion method is virtual and therefore can not be static
    return T{}.v8(isolate).template to<v8pp::class_<T>>();
}


auto javascript::interop::expose_cpp_to_js::expose(
        v8::Isolate* isolate,
        v8::Persistent<v8::Context>& persistent_context,
        javascript::environment::modules::module_type module_type)
        -> void
{
    // derive a local context from the persistent one, and enter it
    v8::Local<v8::Context> local_context = v8::Local<v8::Context>::New(isolate, persistent_context);
    local_context->Enter();

    // TODO : formatting - templating makes me feel ill
    // create the v8 classes from the c++ classes, linked to the current isolate
    auto v8_abort_controller = object_to_v8<dom::aborting::abort_controller>(isolate);
    auto v8_abort_signal = object_to_v8<dom::aborting::abort_signal>(isolate);

    auto v8_custom_event = object_to_v8<dom::events::custom_event>(isolate);
    auto v8_event = object_to_v8<dom::events::event>(isolate);

    auto v8_abstract_iterator = object_to_v8<dom::iterators::abstract_iterator>(isolate);
    auto v8_node_filter = object_to_v8<dom::iterators::node_filter>(isolate);
    auto v8_node_iterator = object_to_v8<dom::iterators::node_iterator>(isolate);
    auto v8_tree_walker = object_to_v8<dom::iterators::tree_walker>(isolate);

    auto v8_mutation_observer = object_to_v8<dom::mutations::mutation_observer>(isolate);
    auto v8_mutation_record = object_to_v8<dom::mutations::mutation_record>(isolate);

    auto v8_attr = object_to_v8<dom::nodes::attr>(isolate);
    auto v8_cdata_section = object_to_v8<dom::nodes::cdata_section>(isolate);
    auto v8_character_data = object_to_v8<dom::nodes::character_data>(isolate);
    auto v8_comment = object_to_v8<dom::nodes::comment>(isolate);
    auto v8_document = object_to_v8<dom::nodes::document>(isolate);
    auto v8_document_fragment = object_to_v8<dom::nodes::document_fragment>(isolate);
    auto v8_document_type = object_to_v8<dom::nodes::document_type>(isolate);
    auto v8_element = object_to_v8<dom::nodes::element>(isolate);
    auto v8_event_target = object_to_v8<dom::nodes::event_target>(isolate);
    auto v8_node = object_to_v8<dom::nodes::node>(isolate);
    auto v8_processing_instruction = object_to_v8<dom::nodes::processing_instruction>(isolate);
    auto v8_shadow_root = object_to_v8<dom::nodes::shadow_root>(isolate);
    auto v8_text = object_to_v8<dom::nodes::text>(isolate);
    auto v8_window_proxy = object_to_v8<dom::nodes::window_proxy>(isolate);
    auto v8_window = object_to_v8<dom::nodes::window>(isolate);
    auto v8_xml_document = object_to_v8<dom::nodes::xml_document>(isolate);

    auto v8_dom_exception = object_to_v8<dom::other::dom_exception>(isolate);
    auto v8_dom_implementation = object_to_v8<dom::other::dom_implementation>(isolate);
    auto v8_xslt_processor = object_to_v8<dom::other::xslt_processor>(isolate);

    auto v8_abstract_range = object_to_v8<dom::ranges::abstract_range>(isolate);
    auto v8_range = object_to_v8<dom::ranges::range>(isolate);
    auto v8_static_range = object_to_v8<dom::ranges::static_range>(isolate);

    auto v8_xpath_evaluator = object_to_v8<dom::xpath::xpath_evaluator>(isolate);
    auto v8_xpath_expression = object_to_v8<dom::xpath::xpath_expression>(isolate);
    auto v8_xpath_result = object_to_v8<dom::xpath::xpath_result>(isolate);

    auto v8_html_anchor_element = object_to_v8<html::elements::html_anchor_element>(isolate);
    auto v8_html_area_element = object_to_v8<html::elements::html_area_element>(isolate);
    auto v8_html_audio_element = object_to_v8<html::elements::html_audio_element>(isolate);
    auto v8_html_base_element = object_to_v8<html::elements::html_base_element>(isolate);
    auto v8_html_body_element = object_to_v8<html::elements::html_body_element>(isolate);
    auto v8_html_br_element = object_to_v8<html::elements::html_br_element>(isolate);
    auto v8_html_button_element = object_to_v8<html::elements::html_button_element>(isolate);
    auto v8_html_canvas_element = object_to_v8<html::elements::html_canvas_element>(isolate);
    auto v8_html_data_element = object_to_v8<html::elements::html_data_element>(isolate);
    auto v8_html_datalist_element = object_to_v8<html::elements::html_datalist_element>(isolate);
    auto v8_html_details_element = object_to_v8<html::elements::html_details_element>(isolate);
    auto v8_html_dialog_element = object_to_v8<html::elements::html_dialog_element>(isolate);
    auto v8_html_div_element = object_to_v8<html::elements::html_div_element>(isolate);
    auto v8_html_dlist_element = object_to_v8<html::elements::html_dlist_element>(isolate);
    auto v8_html_element = object_to_v8<html::elements::html_element>(isolate);
    auto v8_html_form_element = object_to_v8<html::elements::html_form_element>(isolate);
    auto v8_html_head_element = object_to_v8<html::elements::html_head_element>(isolate);
    auto v8_html_heading_element = object_to_v8<html::elements::html_heading_element>(isolate);
    auto v8_html_hr_element = object_to_v8<html::elements::html_hr_element>(isolate);
    auto v8_html_html_element = object_to_v8<html::elements::html_html_element>(isolate);
    auto v8_html_iframe_element = object_to_v8<html::elements::html_iframe_element>(isolate);
    auto v8_html_image_element = object_to_v8<html::elements::html_image_element>(isolate);
    auto v8_html_label_element = object_to_v8<html::elements::html_label_element>(isolate);
    auto v8_html_li_element = object_to_v8<html::elements::html_li_element>(isolate);
    auto v8_html_link_element = object_to_v8<html::elements::html_link_element>(isolate);
    auto v8_html_map_element = object_to_v8<html::elements::html_map_element>(isolate);
    auto v8_html_media_element = object_to_v8<html::elements::html_media_element>(isolate);
    auto v8_html_meta_element = object_to_v8<html::elements::html_meta_element>(isolate);
    auto v8_html_menu_element = object_to_v8<html::elements::html_menu_element>(isolate);
    auto v8_html_mod_element = object_to_v8<html::elements::html_mod_element>(isolate);
    auto v8_html_olist_element = object_to_v8<html::elements::html_olist_element>(isolate);
    auto v8_html_paragraph_element = object_to_v8<html::elements::html_paragraph_element>(isolate);
    auto v8_html_pre_element = object_to_v8<html::elements::html_pre_element>(isolate);
    auto v8_html_picture_element = object_to_v8<html::elements::html_picture_element>(isolate);
    auto v8_html_quote_element = object_to_v8<html::elements::html_quote_element>(isolate);
    auto v8_html_slot_element = object_to_v8<html::elements::html_slot_element>(isolate);
    auto v8_html_source_element = object_to_v8<html::elements::html_source_element>(isolate);
    auto v8_html_span_element = object_to_v8<html::elements::html_span_element>(isolate);
    auto v8_html_style_element = object_to_v8<html::elements::html_style_element>(isolate);
    auto v8_html_table_caption_element = object_to_v8<html::elements::html_table_caption_element>(isolate);
    auto v8_html_table_cell_element = object_to_v8<html::elements::html_table_cell_element>(isolate);
    auto v8_html_table_col_element = object_to_v8<html::elements::html_table_col_element>(isolate);
    auto v8_html_table_element = object_to_v8<html::elements::html_table_element>(isolate);
    auto v8_html_table_row_element = object_to_v8<html::elements::html_table_row_element>(isolate);
    auto v8_html_table_section_element = object_to_v8<html::elements::html_table_section_element>(isolate);
    auto v8_html_title_element = object_to_v8<html::elements::html_title_element>(isolate);
    auto v8_html_track_element = object_to_v8<html::elements::html_track_element>(isolate);
    auto v8_html_time_element = object_to_v8<html::elements::html_time_element>(isolate);
    auto v8_html_ulist_element = object_to_v8<html::elements::html_ulist_element>(isolate);
    auto v8_html_unknown_element = object_to_v8<html::elements::html_unknown_element>(isolate);
    auto v8_html_video_element = object_to_v8<html::elements::html_video_element>(isolate);

    auto v8_html_track_event = object_to_v8<html::events::track_event>(isolate);

    auto v8_html_audio_track = object_to_v8<html::media::audio_track>(isolate);
    auto v8_html_text_track = object_to_v8<html::media::text_track>(isolate);
    auto v8_html_text_track_cue = object_to_v8<html::media::text_track_cue>(isolate);
    auto v8_html_video_track = object_to_v8<html::media::video_track>(isolate);
    auto v8_html_time_ranges = object_to_v8<html::media::time_ranges>(isolate);

    // create the module and an empty module name
    v8pp::module v8_module{isolate};
    v8::Local<v8::String> module_name;

    // expose certain classes depending on the module type (window, worker, etc)
    switch(module_type) {
        case javascript::environment::modules::module_type::window:
        {
            v8_module
                    .class_("AbortController", v8_abort_controller)
                    .class_("AbortSignal", v8_abort_signal)

                    .class_("CustomEvent", v8_custom_event)
                    .class_("Event", v8_event)

                    .class_("NodeFilter", v8_node_filter)
                    .class_("NodeIterator", v8_node_iterator)
                    .class_("TreeWalker", v8_tree_walker)

                    .class_("MutationObserver", v8_mutation_observer)
                    .class_("MutationRecord", v8_mutation_record)

                    .class_("Attr", v8_attr)
                    .class_("CDataSection", v8_cdata_section)
                    .class_("CharacterData", v8_character_data)
                    .class_("Comment", v8_comment)
                    .class_("Document", v8_document)
                    .class_("DocumentFragment", v8_document_fragment)
                    .class_("DocumentType", v8_document_type)
                    .class_("Element", v8_element)
                    .class_("EventTarget", v8_event_target)
//                    .class_("Node", v8_node) TODO -> pure virtual method 'qt()' causing issues here
                    .class_("ProcessingInstruction", v8_processing_instruction)
                    .class_("ShadowRoot", v8_shadow_root)
                    .class_("Text", v8_text)
                    .class_("Window", v8_window)
                    .class_("WindowProxy", v8_window_proxy)
                    .class_("XMLDocument", v8_xml_document)

                    .class_("DOMException", v8_dom_exception)
                    .class_("DOMImplementation", v8_dom_implementation)
                    .class_("XSLTProcessor", v8_xslt_processor)

                    .class_("AbstractRange", v8_abstract_range)
                    .class_("Range", v8_range)
                    .class_("StaticRange", v8_static_range)

                    .class_("XPathEvaluator", v8_xpath_evaluator)
                    .class_("XPathExpression", v8_xpath_expression)
                    .class_("XPathResult", v8_xpath_result)

                    .class_("HTMLAnchorElement", v8_html_anchor_element)
                    .class_("HTMLAreaElement", v8_html_area_element)
                    .class_("HTMLAudioElement", v8_html_audio_element)
                    .class_("HTMLBaseElement", v8_html_base_element)
                    .class_("HTMLBodyElement", v8_html_body_element)
                    .class_("HTMLBRElement", v8_html_br_element)
                    .class_("HTMLButtonElement", v8_html_button_element)
                    .class_("HTMLCanvasElement", v8_html_canvas_element)
                    .class_("HTMLDataElement", v8_html_data_element)
                    .class_("HTMLDataListElement", v8_html_data_element)
                    .class_("HTMLDetailsElement", v8_html_details_element)
                    .class_("HTMLDialogElement", v8_html_dialog_element)
                    .class_("HTMLDivElement", v8_html_div_element)
                    .class_("HTMLDListElement", v8_html_dlist_element)
                    .class_("HTMLElement", v8_html_element)
                    .class_("HTMLFormElement", v8_html_form_element)
                    .class_("HTMLHeadElement", v8_html_head_element)
                    .class_("HTMLHeadingElement", v8_html_heading_element)
                    .class_("HTMLHRElement", v8_html_hr_element)
                    .class_("HTMLHtmlElement", v8_html_html_element)
                    .class_("HTMLIFrameElement", v8_html_iframe_element)
                    .class_("HTMLImageElement", v8_html_image_element)
                    .class_("HTMLLabelElement", v8_html_label_element)
                    .class_("HTMLLIElement", v8_html_li_element)
                    .class_("HTMLLinkElement", v8_html_link_element)
                    .class_("HTMLMapElement", v8_html_map_element)
                    .class_("HTMLMediaElement", v8_html_media_element)
                    .class_("HTMLMetaElement", v8_html_meta_element)
                    .class_("HTMLMenuElement", v8_html_menu_element)
                    .class_("HTMLModElement", v8_html_mod_element)
                    .class_("HTMLOListElement", v8_html_olist_element)
                    .class_("HTMLPictureElement", v8_html_picture_element)
                    .class_("HTMLParagraphElement", v8_html_paragraph_element)
                    .class_("HTMLPreElement", v8_html_pre_element)
                    .class_("HTMLQuoteElement", v8_html_pre_element)
                    .class_("HTMLSlotElement", v8_html_slot_element)
                    .class_("HTMLSourceElement", v8_html_source_element)
                    .class_("HTMLSpanElement", v8_html_span_element)
                    .class_("HTMLStyleElement", v8_html_style_element)
                    .class_("HTMLTableCaptionElement", v8_html_table_caption_element)
                    .class_("HTMLTableCellElement", v8_html_table_cell_element)
                    .class_("HTMLTableColElement", v8_html_table_col_element)
                    .class_("HTMLTableElement", v8_html_table_element)
                    .class_("HTMLTableRowElement", v8_html_table_row_element)
                    .class_("HTMLTableSectionElement", v8_html_table_section_element)
                    .class_("HTMLTitleElement", v8_html_title_element)
                    .class_("HTMLTimeElement", v8_html_time_element)
                    .class_("HTMLTrackElement", v8_html_track_element)
                    .class_("HTMLUListElement", v8_html_ulist_element)
                    .class_("HTMLUnknownElement", v8_html_unknown_element)
                    .class_("HTMLVideoElement", v8_html_video_element)

                    .class_("TrackEvent", v8_html_track_event)

                    .class_("AudioTrack", v8_html_audio_track)
                    .class_("TextTrack", v8_html_text_track)
                    .class_("TextTrackCue", v8_html_text_track_cue)
                    .class_("TimeRanges", v8_html_time_ranges)
                    .class_("VideoTrack", v8_html_video_track)
                    ;

            module_name = v8::String::NewFromUtf8(isolate, "Window").ToLocalChecked();
        }

        case javascript::environment::modules::module_type::worker:
        {
            v8_module
                    .class_("CustomEvent", v8_custom_event)
                    .class_("Event", v8_event)
                    .class_("EventTarget", v8_event_target)
                    .class_("DomException", v8_dom_exception)
                    ;

            module_name = v8::String::NewFromUtf8(isolate, "Worker").ToLocalChecked();
        }

        case javascript::environment::modules::module_type::audio_worklet:
        {
            v8_module
                    .class_("Event", v8_event)
                    .class_("EventTarget", v8_event_target)
                    ;

            module_name = v8::String::NewFromUtf8(isolate, "AudioWorklet").ToLocalChecked();
        }
    }

    // append the module into the context
    // TODO : globalize the classes so Document() can be used instead of Window.Document() - I have a script to do this,
    //  but need to pass in the module name as a parameter)
    local_context->Global()->Set(local_context, module_name, v8_module.new_instance());
}


#endif //SBROWSER_EXPOSE_CPP_TO_JS_HPP
