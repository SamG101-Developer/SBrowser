#ifndef SBROWSER_TEST_V8_HPP
#define SBROWSER_TEST_V8_HPP

#define V8_COMPRESS_POINTERS

#include <iostream>

#include <v8.h>
#include <libplatform/libplatform.h>


void test_v8(int argc, char** argv) {
    v8::V8::InitializeICUDefaultLocation(argv[0]);
    v8::V8::InitializeExternalStartupData(argv[0]);

    auto platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    auto* isolate = v8::Isolate::New(create_params);

    {
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handle_scope(isolate);

        v8::Local<v8::Context> context = v8::Context::New(isolate);
        v8::Context::Scope context_scope(context);

        {
            auto source = v8::String::NewFromUtf8Literal(isolate, "1 + 2;");
            auto script = v8::Script::Compile(context, source).ToLocalChecked();
            auto result = script->Run(context).ToLocalChecked();

            v8::String::Utf8Value utf8(isolate, result);
            std::cout << *utf8 << std::endl;
        }
    }

    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete create_params.array_buffer_allocator;
}


#endif //SBROWSER_TEST_V8_HPP
