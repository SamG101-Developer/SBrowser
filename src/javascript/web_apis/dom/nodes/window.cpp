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
            .template inherit<event_target>()
            .template inherit<ext::listlike<ext::string>>()
            .template function("close", &window::close)
            .template function("stop", &window::stop)
            .template function("focus", &window::focus)
            .template function("open", &window::open)
            .template function("print", &window::print)
            .template function("alert", &window::alert)
            .template function("confirm", &window::confirm)
            .template function("prompt", &window::prompt)
            .template function("postMessage", &window::post_message)
            .template function("move_to", &window::move_to)
            .template function("move_by", &window::move_by)
            .template function("resize_to", &window::resize_to)
            .template function("resize_by", &window::resize_by)
            .template function("getComputedStyle", &window::get_computed_style)
            .template function("navigate", &window::navigate)
            .template var("name", &window::name)
            .template var("status", &window::status)
            .template var("closed", &window::closed)
            .template var("location", &window::location)
            .template var("history", &window::history)
            .template var("customElements", &window::custom_elements)
            .template var("window", &window::window_)
            .template var("self", &window::self)
            .template var("ownerDocument", &window::document)
            .template var("location_bar", &window::location_bar)
            .template var("menu_bar", &window::menu_bar)
            .template var("personal_bar", &window::personal_bar)
            .template var("scroll_bars", &window::scroll_bars)
            .template var("status_bar", &window::status_bar)
            .template var("tool_bar", &window::tool_bar)
            .template var("opener", &window::opener)
            .template var("length", &window::length)
            .template var("frameElement", &window::frame_element)
            .template var("frames", &window::frames)
            .template var("top", &window::top)
            .template var("parent", &window::parent)
            .template var("originAgentCluster", &window::origin_agent_cluster)
            .template var("navigator", &window::navigator)
            .template var("innerWidth", &window::inner_width)
            .template var("innerHeight", &window::inner_height)
            .template var("scrollX", &window::scroll_x)
            .template var("scrollY", &window::scroll_y)
            .template var("pageXOffset", &window::page_x_offset)
            .template var("pageYOffset", &window::page_y_offset)
            .template var("screenX", &window::screen_x)
            .template var("screenY", &window::screen_y)
            .template var("outerWidth", &window::outer_width)
            .template var("outerHeight", &window::outer_height)
            .template var("devicePixelWidth", &window::device_pixel_width)
            .template var("screen", &window::screen)
            .auto_wrap_objects();
}
