#pragma once
#ifndef SBROWSER_STAGES_HPP
#define SBROWSER_STAGES_HPP

#include <v8.h>

#include <javascript/interop/expose_cpp_to_js.hpp>

namespace {static std::unique_ptr<v8::Platform> platform;}

namespace javascript::environment {
    namespace modules {enum module_type;}

    namespace stages {
        void initialize_v8_engine(char** args);
        v8::Isolate* create_isolate();
        v8::Persistent<v8::Context>& create_context(v8::Isolate* isolate, javascript::environment::modules::module_type module_type);
        void execute(v8::Isolate* isolate, v8::Persistent<v8::Context>& persistent_context, const char* code);
        void dispose_isolate(v8::Isolate* isolate);
        void dispose_v8();
    }
}


void javascript::environment::stages::initialize_v8_engine(char** args) {
    v8::V8::InitializeICUDefaultLocation(args[0]);
    v8::V8::InitializeExternalStartupData(args[0]);

    platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();
}


v8::Isolate* javascript::environment::stages::create_isolate() {
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();

    auto* isolate = v8::Isolate::New(create_params);
    return isolate;
}


v8::Persistent<v8::Context>& javascript::environment::stages::create_context(
        v8::Isolate* isolate,
        javascript::environment::modules::module_type module_type) {

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    static v8::Persistent<v8::Context> persistent_context = v8::Persistent<v8::Context>(
            isolate,
            v8::Context::New(isolate));

    javascript::interop::expose_cpp_to_js::expose(isolate, persistent_context, module_type);
    return persistent_context;
}


void javascript::environment::stages::execute(
        v8::Isolate* isolate,
        v8::Persistent<v8::Context>& persistent_context,
        const char* code) {

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    v8::Local<v8::Context> local_context = v8::Local<v8::Context>::New(isolate, persistent_context);
    v8::Context::Scope local_context_scope(local_context);

    {
        v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, code).ToLocalChecked();
        v8::Local<v8::Script> script = v8::Script::Compile(local_context, source).ToLocalChecked();
        v8::Local<v8::Value>  result = script->Run(local_context).ToLocalChecked();

        std::string output = *v8::String::Utf8Value(isolate, result);
        std::cout << output << std::endl;
    }
}


#endif //SBROWSER_STAGES_HPP
