#ifndef SBROWSER_MODULES_HPP
#define SBROWSER_MODULES_HPP


namespace javascript::environment::modules {enum class module_type;}


enum class javascript::environment::modules::module_type
{
    window,
    worker,
    audio_worklet
};


#endif //SBROWSER_MODULES_HPP
