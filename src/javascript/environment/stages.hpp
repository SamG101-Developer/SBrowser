#pragma once
#ifndef SBROWSER_STAGES_HPP
#define SBROWSER_STAGES_HPP

#include <iostream>

#include <javascript/interop/expose_cpp_to_js.hpp>

#include <v8.h>
#include <libplatform/libplatform.h>

namespace {std::unique_ptr<v8::Platform> platform;}

namespace javascript::environment {
    namespace modules {enum module_type;}

    namespace stages {
        func initialize_v8_engine(char** argv) -> void;
        func create_isolate() -> v8::Isolate*;
        func create_context(v8::Isolate* isolate, javascript::environment::modules::module_type module_type) -> v8::Persistent<v8::Context>&;
        func execute(v8::Isolate* isolate, v8::Persistent<v8::Context>& persistent_context, const char* code) -> void;
        func dispose_isolate(v8::Isolate* isolate) -> void;
        func dispose_v8() -> void;
    }
}


func javascript::environment::stages::initialize_v8_engine(char** argv) -> void
{
    // initialize the icu default location and the external startup data
    v8::V8::InitializeICUDefaultLocation(argv[0]);
    v8::V8::InitializeExternalStartupData(argv[0]);

    // initialize the platform
    platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();
}


func javascript::environment::stages::create_isolate() -> v8::Isolate*
{
    // create the create_params object for the array buffer allocator
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();

    // create a new isolate with the create_params object
    auto* isolate = v8::Isolate::New(create_params);
    return isolate;
}


func javascript::environment::stages::create_context(
        v8::Isolate* isolate,
        javascript::environment::modules::module_type module_type)
        -> v8::Persistent<v8::Context>&
{
    // create the isolate_ and handle_scope
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    // create a persistent context from the isolate
    static v8::Persistent<v8::Context> persistent_context = v8::Persistent<v8::Context>{isolate, v8::Context::New(isolate)};

    // expose the web api module code into the javascript context
    javascript::interop::expose_cpp_to_js::expose(isolate, persistent_context, module_type);
    return persistent_context;
}


func javascript::environment::stages::execute(
        v8::Isolate* isolate,
        v8::Persistent<v8::Context>& persistent_context,
        const char* code)
        -> void
{
    // create the isolate_ and handle_scope
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    // derive the local_context, and create a local_context_scope from it
    v8::Local<v8::Context> local_context = v8::Local<v8::Context>::New(isolate, persistent_context);
    v8::Context::Scope local_context_scope(local_context);

    {
        // create the source and script (from compiling the source), and run the script in the local context
        v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, code).ToLocalChecked();
        v8::Local<v8::Script> script = v8::Script::Compile(local_context, source).ToLocalChecked();
        v8::Local<v8::Value>  result = script->Run(local_context).ToLocalChecked();

        // get the output and write it to console
        std::string output = *v8::String::Utf8Value(isolate, result);

        #if DEBUG
        std::cout << output << std::endl;
        #endif
    }
}


#endif //SBROWSER_STAGES_HPP
