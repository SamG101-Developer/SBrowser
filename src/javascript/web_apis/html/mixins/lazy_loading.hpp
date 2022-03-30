#pragma once
#ifndef SBROWSER_LAZY_LOADING_HPP
#define SBROWSER_LAZY_LOADING_HPP

#include <ext/decorators.hpp>
#include <ext/html_property.hpp>

namespace html::mixins {class lazy_loading;}

class html::mixins::lazy_loading
{
public enums:
    enum class lazy_loading_attribute_type {LAZY, EAGER};

public constructors:
    lazy_loading();

public js_properties:
    ext::html_property<ext::string, _T> loading;

private cpp_properties:
    std::function<void()> m_lazy_load_resumption_steps;

private accessors:
    auto set_loading(const ext::string& val) -> void;
};


#endif //SBROWSER_LAZY_LOADING_HPP
