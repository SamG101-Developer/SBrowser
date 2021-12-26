#include <javascript/interop/expose_cpp_to_js.hpp>
#include <javascript/environment/stages.hpp>


int main(int argc, char** argv) {
    javascript::environment::stages::initialize_v8_engine(argv);
    auto* isolate = javascript::environment::stages::create_isolate();
    auto context = javascript::environment::stages::create_context(isolate, javascript::environment::modules::window);

    javascript::environment::stages::execute(isolate, context, "let event = new Window.Event('click', {bubbles: true})");
    javascript::environment::stages::execute(isolate, context, "event.bubbles");

    javascript::environment::stages::dispose_isolate(isolate);
    javascript::environment::stages::dispose_v8();

    return 0;
}

#endif
