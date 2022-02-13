#include "window.hpp"

#include <dom/helpers/shadows.hpp>

#include <dom/nodes/element.hpp>
#include <dom/nodes/document.hpp>

#include <QtWidgets/QScrollArea>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QInputDialog>


dom::nodes::window::window()
        : event_target()
//        , html::mixins::window_or_worker_global_scope()
//        , css::cssom_view::mixins::scrollable()
        , ext::listlike<ext::string>()
{
}


void
dom::nodes::window::alert(ext::cstring& message)
{
    // if the application can show dialogs, get the message
    if (html::helpers::dialogs::cannot_show_dialogs())
        return;

    message = infra::helpers::strings::normalize_newlines(message);
    message = infra::helpers::strings::optionally_truncate(message); // TODO in html spec?

    // create the message box and display the message TODO : remove to render stack
    QMessageBox message_box;
    message_box.setParent(document->render());
    message_box.setIcon(QMessageBox::Icon::Information);
    message_box.setText(message.to_string_qt());
    message_box.addButton(QMessageBox::Ok);
    message_box.setDefaultButton(QMessageBox::Ok);
    message_box.exec();
}


void
dom::nodes::window::confirm(ext::cstring& message) {

    // if the application can show dialogs, get the message
    if (html::helpers::dialogs::cannot_show_dialogs())
        return;

    message = infra::helpers::strings::normalize_newlines(message);
    message = infra::helpers::strings::optionally_truncate(message);

    // create the message box and display the message TODO : remove to render stack
    QMessageBox message_box;
    message_box.setParent(document->render());
    message_box.setIcon(QMessageBox::Icon::Question);
    message_box.setText(message.to_string_qt());
    message_box.addButton(QMessageBox::Cancel);
    message_box.addButton(QMessageBox::Ok);
    message_box.exec();
}


ext::string
dom::nodes::window::prompt(
        ext::cstring& message,
        ext::cstring& default_) {

    // if the application can show dialogs, get the message
    if (html::helpers::dialogs::cannot_show_dialogs())
        return "";

    message = infra::helpers::strings::normalize_newlines(message);
    message = infra::helpers::strings::optionally_truncate(message);
    default_ = infra::helpers::strings::optionally_truncate(default_);

    // create the message box and display the message TODO : remove to render stack
    QInputDialog input_dialog;
    input_dialog.setParent(document->render());
    input_dialog.setLabelText(message);
    input_dialog.setTextValue(default_);
    input_dialog.exec();

    return ext::string{input_dialog.textValue()};
}


css::cssom::other::css_style_declaration*
dom::nodes::window::get_computed_style(
        element* elem,
        ext::cstring& pseudo_element) {

    document* doc = elem->owner_document;
    auto* object = new element{*elem};

    if (not pseudo_element.empty() and pseudo_element.front() == ':')
        ; // TODO

    ext::vector<css::cssom::internal::css_declaration*> declarations{};
    auto* shadow_document = dynamic_cast<document*>(helpers::shadows::shadow_including_root(elem));
    // if (html::internal::browsing_context* browsing_context = shadow_document->m_browsing_context

    css::cssom::other::css_style_declaration style_declaration;
    style_declaration.parent_rule = nullptr;
    style_declaration.owner_node = object;
    style_declaration.m_computed_flag = true;
    style_declaration.m_declarations = declarations;
    return style_declaration;
}


void
dom::nodes::window::move_to(
        long x,
        long y) {

    document->render()->move(x, y);
}


void
dom::nodes::window::move_by(
        long x,
        long y) {

    document->render()->move(
            x - document->render()->x(),
            y - document->render()->y());
}


void
dom::nodes::window::resize_to(
        long width,
        long height) {

    document->render()->resize(width, height);
}


void
dom::nodes::window::resize_by(
        long width,
        long height) {

    document->render()->resize(
            width - document->render()->width(),
            height - document->render()->height());
}


ext::any
dom::nodes::window::v8(v8::Isolate* isolate) const {

    return v8pp::class_<window>{isolate}
            .inherit<event_target>()
            .inherit<ext::listlike<ext::string>>()

            .function("close", &window::close)
            .function("stop", &window::stop)
            .function("focus", &window::focus)
            .function("open", &window::open)

            .function("print", &window::print)
            .function("alert", &window::alert)
            .function("confirm", &window::confirm)
            .function("prompt", &window::prompt)
            .function("postMessage", &window::post_message)

            .function("move_to", &window::move_to)
            .function("move_by", &window::move_by)
            .function("resize_to", &window::resize_to)
            .function("resize_by", &window::resize_by)

            .function("getComputedStyle", &window::get_computed_style)

            .function("navigate", &window::navigate)

            .var("name", &window::name)
            .var("status", &window::status)
            .var("closed", &window::closed)

            .var("location", &window::location)
            .var("history", &window::history)
            .var("customElements", &window::custom_elements)

            .var("window", &window::window_)
            .var("self", &window::self)
            .var("ownerDocument", &window::document)

            .var("location_bar", &window::location_bar)
            .var("menu_bar", &window::menu_bar)
            .var("personal_bar", &window::personal_bar)
            .var("scroll_bars", &window::scroll_bars)
            .var("status_bar", &window::status_bar)
            .var("tool_bar", &window::tool_bar)

            .var("opener", &window::opener)
            .var("length", &window::length)
            .var("frameElement", &window::frame_element)
            .var("frames", &window::frames)
            .var("top", &window::top)
            .var("parent", &window::parent)

            .var("originAgentCluster", &window::origin_agent_cluster)
            .var("navigator", &window::navigator)

            .var("innerWidth", &window::inner_width)
            .var("innerHeight", &window::inner_height)

            .var("scrollX", &window::scroll_x)
            .var("scrollY", &window::scroll_y)
            .var("pageXOffset", &window::page_x_offset)
            .var("pageYOffset", &window::page_y_offset)

            .var("screenX", &window::screen_x)
            .var("screenY", &window::screen_y)
            .var("outerWidth", &window::outer_width)
            .var("outerHeight", &window::outer_height)
            .var("devicePixelWidth", &window::device_pixel_width)

            .var("screen", &window::screen)

            .auto_wrap_objects();
}
