#ifndef SBROWSER_MANUAL_CONVERSIONS_HPP
#define SBROWSER_MANUAL_CONVERSIONS_HPP

#include <ext/string.hpp>
#include <ext/set.hpp>
#include <ext/property.hpp>
#include <ext/any.hpp>
#include <ext/infinity.hpp>

#include <v8pp/convert.hpp>


// TODO -> when from_type/to_type used as parameter types, check if (const) ...& / ...&& should be used


/* FUNCTION */
//template<typename T>
//struct v8pp::convert<std::function<T(...)>> {
//    using from_type = std::function<T(...)>;
//    using to_type = v8::Local<v8::Function>;
//    
//    static auto is_valid(v8::Isolate*, v8::Local<v8::Value> value) -> bool
//    {
//        // verify the value is a non-empty function
//        return not value.IsEmpty() and value->IsFunction();
//    }
//    
//    static auto from_v8(v8::Isolate* isolate, v8::Local<v8::Value> v8_value) -> from_type
//    {
//        // validate the javascript function object
//        if (not is_valid(isolate, v8_value))
//            throw std::invalid_argument("Must be a non-null function");
//        
//        v8::HandleScope handle_scope{isolate};
//        auto cpp_value = std::function{v8_value.template As<v8::Function>().};
//    }
//};


/* STRING */
template<>
struct v8pp::convert<ext::string> {
    using from_type = ext::string;
    using to_type = v8::Local<v8::String>;

    static auto is_valid(v8::Isolate*, v8::Local<v8::Value> value) -> bool
    {
        // verify that the value is a non-empty string
        return not value.IsEmpty() and value->IsString();
    }

    static auto from_v8(v8::Isolate* isolate, v8::Local<v8::Value> v8_value) -> from_type
    {
        // validate the javascript string object
        if (not is_valid(isolate, v8_value))
            throw std::invalid_argument("Must be a non-null string");

        // create the handle_scope and return the c++ string object
        v8::HandleScope handle_scope{isolate};
        auto cpp_value = v8pp::convert<std::string>::from_v8(isolate, v8_value.template As<v8::String>());
        return from_type{cpp_value.c_str()};
    }

    static auto to_v8(v8::Isolate* isolate, const from_type& cpp_value) -> to_type
    {
        // create the handle_scope and return the javascript string object
        v8::EscapableHandleScope escapable_handle_scope{isolate};
        auto v8_string_length = (int)cpp_value.length();
        auto v8_string_type = v8::NewStringType::kNormal;
        auto v8_value = v8::String::NewFromUtf8(isolate, cpp_value.c_str(), v8_string_type, v8_string_length);
        return escapable_handle_scope.Escape(v8_value.ToLocalChecked());
    }
};


/* PROPERTY */
template <typename T>
struct v8pp::convert<ext::property<T>> {
    using from_type = T;
    using to_type = v8::Local<v8::Value>;

    static auto is_valid(v8::Isolate*, v8::Local<v8::Value> value) -> bool
    {
        // verify that the value is non-empty
        return not value.IsEmpty();
    }

    static auto from_v8(v8::Isolate* isolate, to_type v8_value) -> from_type
    {
        // validate the javascript T input
        if (not is_valid(isolate, v8_value))
            throw std::invalid_argument("Must be a non-null value");

        // create the handle_scope and return the c++ property object (returning value invokes property setter)
        v8::HandleScope handle_scope{isolate};
        auto cpp_value = v8pp::convert<T>::from_v8(isolate, v8_value);
        return cpp_value;
    }

    static auto to_v8(v8::Isolate* isolate, const ext::property<T>& cpp_value) -> to_type
    {
        // create the handle_scope and return the javascript T object (T operator invokes property getter)
        v8::EscapableHandleScope escapable_handle_scope{isolate};
        auto v8_value = v8pp::convert<T>::to_v8(isolate, (T)cpp_value);
        return escapable_handle_scope.template Escape(v8_value);
    }
};


template <typename T>
struct v8pp::is_wrapped_class<ext::property<T>> : std::false_type{};


/* VECTOR */
template <typename T>
struct v8pp::convert<ext::vector<T>> {
    using from_type = ext::vector<T>;
    using to_type = v8::Local<v8::Array>;

    static bool is_valid(v8::Isolate* isolate, v8::Local<v8::Value> value)
    {
        // verify that the value is a non-empty array
        return not value.IsEmpty() and value->IsArray();
    }

    static auto from_v8(v8::Isolate* isolate, to_type v8_value) -> from_type
    {
        // validate the javascript array input
        if (not is_valid(isolate, v8_value))
            throw std::invalid_argument("Must be a non-null array");

        // create the handle_scope and return the c++ vector object
        v8::HandleScope handle_scope{isolate};
        auto cpp_value = v8pp::convert<std::vector<T>>::from_v8(isolate, v8_value.template As<v8::Array>());
        return from_type{cpp_value};
    }

    static auto to_v8(v8::Isolate* isolate, const from_type& cpp_value) -> to_type
    {
        // create the handle_scope and return the javascript array object (after filling the array)
        v8::EscapableHandleScope escapable_handle_scope{isolate};

        to_type v8_value_as_array = v8::Array::New(isolate, cpp_value.length());
        for (size_t i = 0; i < cpp_value.length(); ++i)
            v8_value_as_array->Set(isolate->GetCurrentContext(), i, v8pp::convert<T>::to_v8(isolate, cpp_value.at(i)));

        return escapable_handle_scope.template Escape(v8_value_as_array);
    }
};


template <typename T>
struct v8pp::is_wrapped_class<ext::vector<T>> : std::false_type{};


/* SET */
template <typename T>
struct v8pp::convert<ext::set<T>> {
    using from_type = ext::set<T>;
    using to_type = v8::Local<v8::Set>;
    
    static auto is_valid(v8::Isolate* isolate, v8::Local<v8::Value> value) -> bool
    {
        // verify that the value is a non-empty set
        return not value.IsEmpty() and value->IsSet();
    }
    
    static auto from_v8(v8::Isolate* isolate, to_type v8_value) -> from_type
    {
        // validate the javascript set input
        if (not is_valid(isolate, v8_value))
            throw std::invalid_argument("Must be a non-null set");

        // create the handle_scope and return the c++ set object
        v8::HandleScope handle_scope{isolate};
        auto cpp_value = v8pp::convert<std::set<T>>::from_v8(isolate, v8_value.template As<v8::Set>());
        return from_type{cpp_value};
    }


    static auto to_v8(v8::Isolate* isolate, const from_type& cpp_value) -> to_type
    {
        // create the handle_scope and return the javascript set object (after filling the set)
        v8::EscapableHandleScope escapable_handle_scope {isolate};

        to_type v8_value_as_set = v8::Set::New(isolate);
        for (size_t i = 0; i < cpp_value.length(); ++i)
            v8_value_as_set->Set(isolate->GetCurrentContext(), i, v8pp::convert<T>::to_v8(isolate, cpp_value.at(i)));

        return escapable_handle_scope.template Escape(v8_value_as_set);
    }
};


template <typename T>
struct v8pp::is_wrapped_class<ext::set<T>> : std::false_type{};


/* INFINITY */
template <typename T>
struct v8pp::convert<ext::infinity<T>> {
    using from_type = ext::infinity<T>;
    using to_type = v8::Local<v8::Number>;

    static auto is_valid(v8::Isolate* isolate, v8::Local<v8::Value> value) -> bool
    {
        // verify that the value is a non-empty number
        return not value.IsEmpty() and value->IsNumber();
    }

    static auto from_v8(v8::Isolate* isolate, v8::Local<v8::Value> v8_value) -> from_type
    {
        // validate the javascript number input
        if (not is_valid(isolate, v8_value))
            throw std::invalid_argument("Must be a non-null number");

        // create the handle_scope and return the c++ object
        v8::HandleScope handle_scope{isolate};
        auto cpp_value = v8pp::convert<T>::from_v8(v8_value.template As<v8::Number>());
        return from_type{.m_positive=cpp_value >= 0.0};
    }

    static auto to_v8(v8::Isolate* isolate, const from_type& cpp_value) -> to_type
    {
        // create the handle_scope and return the javascript number object (T operator invokes numeric conversion)
        v8::EscapableHandleScope escapable_handle_scope{isolate};
        auto v8_value = v8pp::convert<T>::to_v8(isolate, (T)cpp_value);
        return escapable_handle_scope.Escape(v8_value);
    }
};


template <typename T>
struct v8pp::is_wrapped_class<ext::infinity<T>> : std::false_type{};


/* ANY */
template<>
struct v8pp::convert<ext::any> { // TODO -> Date, Maps, Infinity, NaN, GlobalThis, Function, Symbol, Error, Math, RegExp, Buffers, Atomics, DataView, JSON, Promise, Generators/Functions, Reflect, Proxy, Intl., WebAssembly., Arguments
    using from_type = ext::any;
    using to_type = v8::Local<v8::Value>;

    static auto is_valid(v8::Isolate*, v8::Local<v8::Value> value) -> bool
    {
        // no verifications
        return true;
    }

    static auto from_v8(v8::Isolate* isolate, v8::Local<v8::Value> v8_value) -> from_type
    {
        // call is_valid to keep uniform with other struct implementations (can never throw)
        if (not is_valid(isolate, v8_value))
            throw std::invalid_argument("Impossible Error");

        // create the handle_scope and return the c++ object
        v8::HandleScope handle_scope{isolate};
        if (v8_value->IsBoolean())
            return from_type{v8pp::convert<bool>::from_v8(isolate, v8_value)};

        else if (v8_value->IsNumber() or v8_value->IsBigInt())
            return from_type{v8pp::convert<long double>::from_v8(isolate, v8_value)};

        else if (v8_value->IsString())
            return from_type{v8pp::convert<ext::string>::from_v8(isolate, v8_value)};

        else if (v8_value->IsUndefined())
            return from_type{};

        else if (v8_value->IsNull())
        {
            auto f = from_type{};
            f.emplace<void>();
            return f;
        }

        else
            throw std::invalid_argument("Invalid type");
    }

    static auto to_v8(v8::Isolate* isolate, const from_type& cpp_value) -> to_type
    {
        // create the handle_scope and return the javascript number object (T operator invokes numeric conversion)
        v8::EscapableHandleScope escapable_handle_scope{isolate};
        if (cpp_value.type() == typeid(bool)) // BOOLEAN
            return escapable_handle_scope.Escape(v8pp::convert<bool>::to_v8(isolate, cpp_value.to<bool>()));

        else if (cpp_value.is_numeric()) // NUMBER
            return escapable_handle_scope.Escape(v8pp::convert<double>::to_v8(isolate, cpp_value.to<double>()));

        else if (cpp_value.type() == typeid(ext::string)) // STRING
            return escapable_handle_scope.Escape(v8pp::convert<ext::string>::to_v8(isolate, cpp_value.to<ext::string>()));

        else if (cpp_value.type() == typeid(ext::vector<ext::any>)) // ARRAY TODO -> does this work?
            return escapable_handle_scope.Escape(v8pp::convert<ext::vector<ext::any>>::to_v8(isolate, cpp_value.to<ext::vector<ext::any>>()));

        else if (cpp_value.empty()) // UNDEFINED
            return escapable_handle_scope.Escape(v8::Undefined(isolate));

        else if (cpp_value.type() == typeid(void)) // NULL
            return escapable_handle_scope.Escape(v8::Null(isolate));

        else // OBJECT
            return escapable_handle_scope.Escape(v8::Object::New(isolate));
    }
};


#endif //SBROWSER_MANUAL_CONVERSIONS_HPP
