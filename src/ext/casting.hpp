#ifndef SBROWSER_CASTING_HPP
#define SBROWSER_CASTING_HPP

template <typename ...Args, typename U>
auto multi_cast(
        U* cls)
-> bool
{
    // check if an object matches any of the types in Args
    return (... ||(nullptr != dynamic_cast<Args>(cls)));
}


#endif //SBROWSER_CASTING_HPP
