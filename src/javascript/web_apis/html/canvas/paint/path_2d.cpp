#include "path_2d.hpp"


auto html::canvas::paint::path_2d::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    #define CTOR_T_1 path_2d*
    #define CTOR_T_2 const ext::string&

    return v8pp::class_<path_2d>{isolate}
            .ctor<CTOR_T_1>()
            .ctor<CTOR_T_2>()
            .inherit<mixins::canvas_path<path_2d>>()
            .function("addPath", &path_2d::add_path)
            .auto_wrap_objects();

    #undef CTOR_T_1
    #undef CTOR_T_1
}
