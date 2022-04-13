#ifndef SBROWSER_MODULES_HPP
#define SBROWSER_MODULES_HPP


namespace javascript::environment::modules {enum class module_type;}


enum class javascript::environment::modules::module_type
{
    WINDOW,
    WORKER,
    AUDIO_WORKLET,
    PAINT_WORKLET
};


#endif //SBROWSER_MODULES_HPP
