#ifndef SBROWSER_CASTING_HPP
#define SBROWSER_CASTING_HPP

template <typename ...Args, typename U>
auto multi_cast(
        const U* const cls)
        -> bool
{
    // check if an object matches any of the types in Args
    return ((dynamic_cast<Args>(cls)) || ...);
}


#endif //SBROWSER_CASTING_HPP
