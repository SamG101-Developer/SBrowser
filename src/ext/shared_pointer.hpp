#ifndef SBROWSER_SHARED_POINTER_HPP
#define SBROWSER_SHARED_POINTER_HPP

namespace ext {template <typename T> class shared_pointer;}
namespace ext {template <typename T, typename ...Args> auto make_shared(Args&&... args) -> shared_pointer<T>;}

#include <utility>
#include <ext/decorators.hpp>


template <typename T>
class ext::shared_pointer
{
public aliases:
    using pointer_t = T;

public constructors:
    shared_pointer();
    shared_pointer(T* that);
    shared_pointer(const shared_pointer& that);
    shared_pointer(shared_pointer&& that) noexcept;
    ~shared_pointer();

    auto operator=(const shared_pointer<T>& that) -> shared_pointer&;
    auto operator=(shared_pointer<T>&& that) noexcept -> shared_pointer&;

public cpp_methods:
    auto is_unique() const -> bool;
    auto swap(shared_pointer& that);
    auto get() const -> T*;
    auto reset() -> void;

public operators:
    auto operator* () const -> T&;
    auto operator->() const -> T*;
    auto operator< (const shared_pointer& that) const -> bool;
    operator bool() const;
    operator T*() const;

private nested_class:
    class counter
    {
    public cpp_methods:
        auto add_reference   () -> size_t {return ++m_counter;};
        auto remove_reference() -> size_t {return --m_counter;};

        [[nodiscard]] auto get_reference() const -> size_t {return m_counter;};

    private cpp_properties:
        size_t m_counter;
    };

private cpp_methods:
    auto release() -> void;
    auto copy_smart_pointer(const shared_pointer& that) -> void;
    auto move_smart_pointer(shared_pointer&& that) noexcept -> void;

private cpp_properties:
    T* m_pointer;
    counter* m_counter;
};


template <typename T>
ext::shared_pointer<T>::shared_pointer()
        : m_pointer{nullptr}
        , m_counter{nullptr}
{
    // for an empty pointer, initialize the counter and pointer to null (no need for a counter for a nullptr)
}


template <typename T>
ext::shared_pointer<T>::shared_pointer(T* that)
        : m_counter{new counter{}}
        , m_pointer{that}
{
    // for a given raw pointer, initialize the counter to a new counter and the pointer to the raw pointer
    // add a new reference to the counter object (default increment due to pointer != nullptr now)
    m_counter->add_reference();
}


template <typename T>
ext::shared_pointer<T>::shared_pointer(
        const shared_pointer<T>& that)
{
    // call the internal copy_smart_pointer method, to handle pointer copying
    copy_smart_pointer(that);
}


template <typename T>
ext::shared_pointer<T>::shared_pointer(
        shared_pointer<T>&& that) noexcept
{
    // call the internal move_smart_pointer method, to handle pointer moving (forward smart pointer into the method)
    move_smart_pointer(std::forward<T>(that));
}


template <typename T>
ext::shared_pointer<T>::~shared_pointer()
{
    // when a smart pointer is destroyed, call the release method, to handle resources and the counter object
    release();
}


template <typename T>
auto ext::shared_pointer<T>::operator=(
        const shared_pointer<T>& that)
        -> shared_pointer<T>&
{
    // process if the new smart pointer has a non-nullptr raw pointer, and the internal raw pointers are different
    if (that.m_pointer and m_pointer != that.m_pointer)
    {
        // release the current pointer, and call the internal call_smart_pointer method
        release();
        copy_smart_pointer(that);
    }

    // return a reference to this smart pointer
    return *this;
}


template <typename T>
auto ext::shared_pointer<T>::operator=(
        shared_pointer<T>&& that) noexcept
        -> shared_pointer<T>&
{
    // process if the new smart pointer has a non-nullptr raw pointer, and the raw pointers are different
    if (that.m_pointer and m_pointer != that.m_pointer)
    {
        // release the current pointer, and call tne internal move_smart_pointer methods
        release();
        move_smart_pointer(std::forward<T>(that));
    }

    // return a reference to this smart pointer
    return *this;
}


template <typename T>
auto ext::shared_pointer<T>::is_unique(
        ) const
        -> bool
{
    // a smart pointer is unique if only 1 smart pointer points to the raw pointer being stored in it
    return m_counter->get_reference() == 1;
}


template <typename T>
auto ext::shared_pointer<T>::swap(
        shared_pointer<T>& that)
{
    // swap the counter and internal raw pointer when this smart pointer is swapped with another one
    std::swap(m_counter, that.m_counter);
    std::swap(m_pointer, that.m_pointer);
}


template <typename T>
auto ext::shared_pointer<T>::get(
        ) const
        -> T*
{
    // access the raw pointer with a method call syntax
    return m_pointer;
}


template <typename T>
auto ext::shared_pointer<T>::reset()
        -> void
{
    // clear the pointer being stored
    // TODO - check counter and handle differently if decrement == 0?
    m_pointer = nullptr;
}


template <typename T>
auto ext::shared_pointer<T>::operator*(
        ) const
        -> T&
{
    // access the raw pointer with the "*" operator
    return *m_pointer;
}


template <typename T>
auto ext::shared_pointer<T>::operator->(
        ) const
        -> T*
{
    // access the attributes of the raw pointer with the "->" operator
    return m_pointer;
}


template <typename T>
auto ext::shared_pointer<T>::operator<(
        const shared_pointer <T>& that) const
        -> bool
{
    // compare by addressed of internal raw pointers
    return m_pointer < that.m_pointer;
}


template <typename T>
ext::shared_pointer<T>::operator bool(
        ) const
{
    // the boolean conversion checks whether the internal raw pointer is nullptr (false) or not (true)
    return m_pointer != nullptr;
}


template <typename T>
ext::shared_pointer<T>::operator T*(
        ) const
{
    // the T* conversion operator is used for passing a smart pointer as a parameter to a raw pointer type
    return m_pointer;
}


template <typename T>
auto ext::shared_pointer<T>::release()
        -> void
{
    // check the counter exists, and removing this object from the counter results in no objects pointing to the raw pointer
    if (m_counter and m_counter->remove_reference() == 0)
    {
        // delete the pointer and counter, and set them to nullptr (initialized)
        delete m_pointer; m_pointer = nullptr;
        delete m_counter; m_counter = nullptr;
    }
}


template <typename T>
auto ext::shared_pointer<T>::copy_smart_pointer(
        const shared_pointer<T>& that)
        -> void
{
    // copy the pointer and counter into this object, and increment the counter (new object exists)
    m_pointer = that.m_pointer;
    m_counter = that.m_counter;
    m_counter->add_reference();
}


template <typename T>
auto ext::shared_pointer<T>::move_smart_pointer(
        shared_pointer<T>&& that) noexcept
        -> void
{
    // move the pointer and counter into this object, and release resources from the other object
    m_pointer = that.m_pointer;
    m_counter = that.m_counter;
    that.release();
}


template <typename T, typename ...Args>
auto ext::make_shared(
        Args&& ...args)
        -> shared_pointer<T>
{
    return shared_pointer(new T{std::forward<Args>(args)...});
}


#endif //SBROWSER_SHARED_POINTER_HPP
