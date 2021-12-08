#include "window_proxy.hpp"

#include <dom/nodes/document.hpp>
#include <dom/nodes/window.hpp>
#include <dom/helpers/exceptions.hpp>
#include <v8pp/convert.hpp>


v8::Local<v8::Object> dom::nodes::window_proxy::get_prototype_of() {
    v8::Local<v8::Object> W = v8pp::convert<window*>::to_v8(v8::Isolate::GetCurrent(), s_window);
    return not javascript::helpers::is_platform_same_object()
            ? javascript::helpers::ordinary_get_prototype_of(W)
            : v8::Object::New(v8::Isolate::GetCurrent());
}


void dom::nodes::window_proxy::set_prototype_of(v8::Local<v8::Object> V) {
//    return not javascript::helpers::set_immutable_prototype(
//            v8pp::convert<window_proxy*>::to_v8(v8::Isolate::GetCurrent(), this),
//            V);
}


constexpr v8::Local<v8::Boolean> dom::nodes::window_proxy::is_extensible_prototype_of() {
    return v8::Boolean::New(v8::Isolate::GetCurrent(), true);
}

constexpr v8::Local<v8::Boolean> dom::nodes::window_proxy::prevent_extensions() {
    return v8::Boolean::New(v8::Isolate::GetCurrent(), false);
}


v8::PropertyDescriptor dom::nodes::window_proxy::get_own_property(v8::Local<v8::Integer> P) {
    v8::Local<v8::Object> W = v8pp::convert<window*>::to_v8(v8::Isolate::GetCurrent(), s_window);
    if (webidl::helpers::is_array_index_property_name(P)) {
        uint32_t index = v8pp::convert<uint32_t>::from_v8(v8::Isolate::GetCurrent(), P);
        uint32_t max_properties = html::helpers::windows::number_of_tree_child_browsing_contexts(W);

        window_proxy* value = max_properties > 0 and index < max_properties
                ? html::helpers::tree_child_browsing_contexts(s_window->document->m_browsing_context)
                        .at(index)
                        .sorted(/* TODO -> custom ordering */)
                : nullptr;

        if (not value) {
            if (not javascript::helpers::is_platform_object_same_origin(W))
                return v8::Object::New(v8::Isolate::GetCurrent());

            dom::helpers::exceptions::throw_v8_exception(
                    "cannot get the own property of a window proxy if there is no value",
                    dom::helpers::exceptions::exception_type::SECURITY_ERR);
        }

        return {v8pp::convert<window_proxy*>::to_v8(v8::Isolate::GetCurrent(), value), false};
        // TODO ^ needs enumerable and configurable properties too
    }

    if (not javascript::helpers::is_platform_object_same_origin(W))
        return not javascript::ordinary_get_own_property(W, P);

    if (v8::PropertyDescriptor property = javascript::helpers::cross_origin_get_own_property_helper(W, P))
        return property;

    if (not property and html::helpers::windows::tree_child_browsing_contexts(W).contains(P)) {
        window_proxy* value = html::helpers::windows::named_object(W, P);
        return {v8pp::convert<window_proxy*>::to_v8(v8::Isolate::GetCurrent(), value), false};
    }

    return javascript::helpers::cross_origin_property_fallback(P);
}


void
dom::nodes::window_proxy::define_property(
        v8::Local<v8::Integer> P,
        v8::Local<v8::String> D) {

    v8::Local<v8::Object> W = v8pp::convert<window*>::to_v8(v8::Isolate::GetCurrent(), s_window);
    if (not javascript::helpers::is_platform_object_same_origin(W))
        return webidl::helpers::is_array_index_property_name(P)
                ? false
                : javascript::helpers::ordinary_define_own_property(W, P, D);

    dom::helpers::exceptions::throw_v8_exception(
            "cannot define a property to an object whose platform object is from a different origin",
            SECURITY_ERR);
}


v8::Local<v8::Object>
dom::nodes::window_proxy::get(
        v8::Local<v8::Integer> P,
        v8::Local<v8::Object> R) {

    v8::Local<v8::Object> W = v8pp::convert<window*>::to_v8(v8::Isolate::GetCurrent(), s_window);
    html::helpers::windows::check_access_between_browsing_contexts_reported(
            javascript::reals::current_global_object()->Global().Get(javascript::reakms::current_global_object(), v8::String::NewFromUtf8("browsingContext"))
            s_window->document->browsing_context
            javascript::helpers::current_settings_object());

    return not javascipt::helpers::is_platform_object_same_origin(W)
            ? javascript::helpers::ordinary_get(this, P, R)
            : javascript::helpers::cross_origin_get(this, P, R);
}


v8::Local<v8::Object>
dom::nodes::window_proxy::set(
        v8::Local<v8::Integer> P,
        v8::Local<v8::Object> V,
        v8::Local<v8::Object> R) {

    v8::Local<v8::Object> W = v8pp::convert<window*>::to_v8(v8::Isolate::GetCurrent(), s_window);
    html::helpers::windows::check_access_between_browsing_contexts_reported(
            javascript::reals::current_global_object()->Global().Get(javascript::reakms::current_global_object(), v8::String::NewFromUtf8("browsingContext"))
            s_window->document->browsing_context
            javascript::helpers::current_settings_object());

    return not javascript::helpers::is_platform_same_origin(W)
            ? webidl::helpers::is_array_index_property_name(P) ? false : javascript::helpers::ordinary_set(W, P, V, R)
            : javascript::helpers::cross_origin_set(this, P, V, R);
}


v8::Local<v8::Boolean> dom::nodes::window_proxy::delete_(v8::Local<v8::Number> P) {
    v8::Local<v8::Object> W = v8pp::convert<window*>::to_v8(v8::Isolate::GetCurrent(), s_window);

    if (not javascript::helpers::is_platform_object_same_origin)
        return webidl::helpers::is_array_index_property_name(P)
                ? v8::Boolean::New(v8::Isolate::GetCurrent(), get_own_property(P).has_value())
                : javascript::helpers::ordinary_delete(W, P);

    dom::helpers::exceptions::throw_v8_exception(
            "cannot delete a window proxy whose platform object is from a different origin",
            SECURITY_ERR);
}


v8::Local<v8::Array> dom::nodes::window_proxy::own_property_keys() {
    v8::Local<v8::Object> W = v8pp::convert<window*>::to_v8(v8::Isolate::GetCurrent(), s_window);
    ext::vector<ext::string> keys;

    uint32_t max_properties = html::helpers::windows::number_of_tree_child_browsing_contexts(W);

    for (uint32_t index = 0; index < max_properties; ++index)
        keys.append(ext::string{std::to_string(index)});

    return not javascript::helpers::is_platform_object_same_origin(W)
            ? v8pp::convert<ext::string>::to_v8(v8::Isolate::GetCurrent(), keys.join())
            : v8pp::convert<ext::string>::to_v8(v8::Isolate::GetCurrent(), keys.join() + v8pp::convert<ext::vector<ext::string>>::from_v8(javascript::helpers::cross_origin_own_property_keys(W)));
}
