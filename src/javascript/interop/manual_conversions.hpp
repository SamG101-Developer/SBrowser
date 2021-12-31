#ifndef SBROWSER_MANUAL_CONVERSIONS_HPP
#define SBROWSER_MANUAL_CONVERSIONS_HPP

#include <ext/iterables/string.hpp>
#include <ext/types/any.hpp>
#include <ext/types/infinity.hpp>
#include <ext/types/property.hpp>

#include <v8pp/convert.hpp>


template<>
struct v8pp::convert<ext::string> {
    using from_type = ext::string;
    using to_type = v8::Local<v8::String>;

    static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value) {return not value.IsEmpty() and value->IsString();}

    static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> v8_value) {
        if (not is_valid(isolate, v8_value)) throw std::invalid_argument("Must be a non-null string");

        v8::HandleScope handle_scope(isolate);
        return from_type{v8pp::convert<std::string>::from_v8(isolate, v8_value.As<v8::String>())};
    }

    static to_type to_v8(v8::Isolate* isolate, const from_type& cpp_value) {
        v8::EscapableHandleScope escapable_handle_scope(isolate);
        return escapable_handle_scope.Escape(v8::String::NewFromUtf8(isolate, cpp_value, v8::NewStringType::kNormal, cpp_value.length()).ToLocalChecked());
    }
};


template <typename T>
struct v8pp::convert<ext::dom_property<T>> {
    using from_type = T;
    using to_type = v8::Local<v8::Value>;

    static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value) {return not value.IsEmpty();}

    static from_type from_v8(v8::Isolate* isolate, to_type v8_value) {
        if (not is_valid(isolate, v8_value)) throw std::invalid_argument("Must be a non-null value");

        v8::HandleScope handle_scope(isolate);
        return v8pp::convert<T>::from_v8(isolate, v8_value);
    }

    static to_type to_v8(v8::Isolate* isolate, const ext::dom_property<T>& cpp_value) {
        v8::EscapableHandleScope escapable_handle_scope(isolate);
        return escapable_handle_scope.Escape(v8pp::convert<T>::to_v8(isolate, (T)cpp_value));
    }
};


template <typename T>
struct v8pp::is_wrapped_class<ext::dom_property<T>> : std::false_type{};


template <typename T>
struct v8pp::convert<ext::vector<T>> {
    using from_type = ext::vector<T>;
    using to_type = v8::Local<v8::Array>;

    static bool is_valid(v8::Isolate* isolate, v8::Local<v8::Value> value) {return not value.IsEmpty() and value->IsArray();}

    static from_type from_v8(v8::Isolate* isolate, to_type v8_value) {
        if (not is_valid(isolate, v8_value)) throw std::invalid_argument("Must be a non-null array");

        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Array> v8_value_as_array = v8_value.As<v8::Array>();

        from_type cpp_value{};
        for (size_t i = 0; i < v8_value_as_array->Length(); ++i)
            cpp_value.append(v8pp::convert<T>::from_v8(isolate, v8_value_as_array->Get(isolate->GetCurrentContext(), i).ToLocalChecked()));
        return cpp_value;
    }

    static to_type to_v8(v8::Isolate* isolate, const from_type& cpp_value) {
        v8::EscapableHandleScope escapable_handle_scope(isolate);

        to_type v8_value_as_array = v8::Array::New(isolate, cpp_value.length());
        for (size_t i = 0; i < cpp_value.length(); ++i)
            v8_value_as_array->Set(isolate->GetCurrentContext(), i, v8pp::convert<T>::to_v8(isolate, cpp_value.at(i)));
        return v8_value_as_array;
    }
};


template <typename T>
struct v8pp::is_wrapped_class<ext::vector<T>> : std::false_type{};


template <typename T>
struct v8pp::convert<ext::infinity<T>> {
    using from_type = ext::infinity<T>;
    using to_type = v8::Local<v8::Number>;

    static bool is_valid(v8::Isolate* isolate, v8::Local<v8::Value> value) {return not value.IsEmpty() and value->IsNumber();}

    static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> v8_value) {
        if (not is_valid(isolate, v8_value)) throw std::invalid_argument("Must be a non-null number");

        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Number> v8_value_as_number = v8_value.As<v8::Number>();

        return from_type{.m_positive=v8pp::convert<T>::from_v8(v8_value_as_number) >= 0};
    }

    static to_type to_v8(v8::Isolate* isolate, const from_type& cpp_value) {
        v8::EscapableHandleScope escapable_handle_scope(isolate);
        return escapable_handle_scope.Escape(v8pp::convert<T>::to_v8(isolate, (T)cpp_value));
    }
};


template <typename T>
struct v8pp::is_wrapped_class<ext::infinity<T>> : std::false_type{};


template<>
struct v8pp::convert<ext::any> { // TODO -> Date, Maps, Infinity, NaN, GlobalThis, Function, Symbol, Error, Math, RegExp, Buffers, Atomics, DataView, JSON, Promise, Generators/Functions, Reflect, Proxy, Intl., WebAssembly., Arguments
    using from_type = ext::any;
    using to_type = v8::Local<v8::Value>;

    static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value) {return true;}

    static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> v8_value) {
        if (not is_valid(isolate, v8_value)) throw std::invalid_argument("Impossible Error");

        v8::HandleScope handle_scope(isolate);
        if (v8_value->IsBoolean()) // BOOLEAN
            return from_type{v8pp::convert<bool>::from_v8(isolate, v8_value)};

        else if (v8_value->IsNull()) { // NULL
            auto f = from_type{}; f.emplace<void>(); return f;}

        else if (v8_value->IsUndefined()) // UNDEFINED
            return from_type{};

        else if (v8_value->IsNumber() or v8_value->IsBigInt()) // NUMBER
            return from_type{v8pp::convert<long double>::from_v8(isolate, v8_value)};

        else if (v8_value->IsString()) // STRING
            return from_type{v8pp::convert<ext::string>::from_v8(isolate, v8_value)};

        else // OBJECT
            throw std::invalid_argument("Invalid type");
    }

    static to_type to_v8(v8::Isolate* isolate, const from_type& cpp_value) {
        v8::EscapableHandleScope escapable_handle_scope(isolate);
        if (cpp_value.type() == typeid(bool)) // BOOLEAN
            return escapable_handle_scope.Escape(v8pp::convert<bool>::to_v8(isolate, (bool)cpp_value));

        else if (cpp_value.type() == typeid(void)) // NULL
            return escapable_handle_scope.Escape(v8::Null(isolate));

        else if (cpp_value.empty()) // UNDEFINED
            return escapable_handle_scope.Escape(v8::Undefined(isolate));

        else if (cpp_value.is_numeric()) // NUMBER (no bigint, +-infinity, NaN)
            return escapable_handle_scope.Escape(v8pp::convert<double>::to_v8(isolate, (double)cpp_value));

        else if (cpp_value.type() == typeid(ext::string)) // STRING
            return escapable_handle_scope.Escape(v8pp::convert<ext::string>::to_v8(isolate, (ext::string)cpp_value));

        else if (cpp_value.type() == typeid(ext::vector<ext::any>)) // ARRAY TODO -> does this work?
            return escapable_handle_scope.Escape(v8pp::convert<ext::vector<ext::any>>::to_v8(isolate, (ext::vector<ext::any>)cpp_value));

        else // OBJECT
            return escapable_handle_scope.Escape(v8::Object::New(isolate));
    }
};



#endif //SBROWSER_MANUAL_CONVERSIONS_HPP
