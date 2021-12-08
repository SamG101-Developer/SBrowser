#include "window.hpp"

#include <dom/nodes/element.hpp>
#include <dom/nodes/document.hpp>

#include <QtWidgets/QScrollArea>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QInputDialog>


void
dom::nodes::window::alert(ext::cstring& message) {

    if (html::helpers::dialogs::cannot_show_dialogs()) return;
    message = infra::helpers::strings::normalize_newlines(message);
    message = infra::helpers::strings::optionally_truncate(message); // TODO in html spec?

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

    if (html::helpers::dialogs::cannot_show_dialogs()) return;
    message = infra::helpers::strings::normalize_newlines(message);
    message = infra::helpers::strings::optionally_truncate(message);

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

    if (html::helpers::dialogs::cannot_show_dialogs()) return;
    message = infra::helpers::strings::normalize_newlines(message);
    message = infra::helpers::strings::optionally_truncate(message);
    default_ = infra::helpers::strings::optionally_truncate(default_);

    QInputDialog input_dialog;
    input_dialog.setParent(document->render());
    input_dialog.setLabelText(message.to_string_qt());
    input_dialog.setTextValue(default_.to_string_qt());
    input_dialog.exec();

    return ext::string{input_dialog.textValue()};
}


css::cssom::other::css_style_declaration*
dom::nodes::window::get_computed_style(
        element* elem,
        ext::cstring& pseudo_element) {

    document* doc = elem->owner_document;
    auto* object = new element{*elem};

    if (not pseudo_element.empty() and pseudo_element.front() == *":")
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
