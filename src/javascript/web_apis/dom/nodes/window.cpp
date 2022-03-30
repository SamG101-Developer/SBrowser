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


auto dom::nodes::window::alert(
        const ext::string& message) const
        -> void
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
    message_box.setText(message);
    message_box.addButton(QMessageBox::Ok);
    message_box.setDefaultButton(QMessageBox::Ok);
    message_box.exec();
}


auto dom::nodes::window::confirm(
        const ext::string& message) const
        -> void
{
    // if the application can show dialogs, get the message
    if (html::helpers::dialogs::cannot_show_dialogs())
        return;

    message = infra::helpers::strings::normalize_newlines(message);
    message = infra::helpers::strings::optionally_truncate(message);

    // create the message box and display the message TODO : remove to render stack
    QMessageBox message_box;
    message_box.setParent(document->render());
    message_box.setIcon(QMessageBox::Icon::Question);
    message_box.setText(message);
    message_box.addButton(QMessageBox::Cancel);
    message_box.addButton(QMessageBox::Ok);
    message_box.exec();
}


auto dom::nodes::window::prompt(
        const ext::string& message,
        const ext::string& default_) const
        -> ext::string
{
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


auto dom::nodes::window::get_computed_style(
        element* elem,
        const ext::string& pseudo_element)
        -> css::cssom::other::css_style_declaration*
{

    document* doc = elem->owner_document;
    const auto* const object = new element{*elem};

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


auto dom::nodes::window::move_to(
        const long x,
        const long y)
        -> void
{
    document->render()->move(x, y);
}


auto dom::nodes::window::move_by(
        const long x,
        const long y)
        -> void
{
    document->render()->move(
            x - document->render()->x(),
            y - document->render()->y());
}


auto dom::nodes::window::resize_to(
        const long width,
        const long height)
        -> void
{
    document->render()->resize(width, height);
}


auto dom::nodes::window::resize_by(
        const long width,
        const long height)
        -> void
{
    document->render()->resize(
            width - document->render()->width(),
            height - document->render()->height());
}


auto dom::nodes::window::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
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
