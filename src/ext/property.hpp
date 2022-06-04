#ifndef SBROWSER_PROPERTY_HPP
#define SBROWSER_PROPERTY_HPP

#include <algorithm>
#include <any>
#include <iostream>
#include <functional>
#include <set>
#include <utility>

#include <ext/type_traits.hpp>

namespace ext {template <typename T, bool ce_reactions> class property;}


#define bind_get(p, method) p._meta.getter  = [this] -> typename decltype(p)::value_type {return method();}
#define bind_set(p, method) p._meta.setter  = [this](auto&& that) {method(std::forward<decltype(that)>(that));}
#define bind_del(p, method) p._meta.deleter = [this] {method();}

#define unlock_property(property_name) property_name._meta.unlock(); {
#define lock_property(property_name) } property_name._meta.lock();


#define VERIFY_LOCK                                                        \
    if (_meta.m_locked)                                                    \
    {                                                                      \
        const char* error_message = "Property raw value not accessible\n"; \
        std::cerr << error_message;                                        \
        throw std::runtime_error{error_message};                           \
    }


template <typename T, bool ce_reactions = false>
class ext::property
{
public:
    class meta
    {
        using qt_method_t     = std::function<void(T)>;
        using qt_method_set_t = std::set<qt_method_t>;
        friend class property<T>;

    public:
        // getter and setter functions for the property
        std::function<T   ( )> getter  = [this](      ) {return m_value;};
        std::function<void(T)> setter  = [this](T that) {m_value = that;};
        std::function<void( )> deleter = [this](      ) {if constexpr(std::is_pointer_v<T>) {delete m_value; m_value = nullptr;}};

        // unlock and lock access to the raw property value
        auto constexpr unlock() -> void {m_locked = false;}
        auto constexpr lock  () -> void {m_locked = true ;}

        // attach a clamp to the value
        auto attach_clamp(T&& low, T&& high) -> void requires std::is_arithmetic_v<T>
        {
            // attach the clamp and clamp the value if it needs clamping
            m_clamp.is    = true;
            m_clamp.range = std::pair{std::forward<T>(low), std::forward<T>(high)};
            setter(m_value);
        }

        // attach constraints to the value
        template <typename ...Args>
        auto attach_constraint(Args&&... constraints) -> void
        {
            // attach the constraints (doesn't change value if not in the constraints already)
            m_constraints.is     = true;
            m_constraints.values = std::set{std::forward<Args>(constraints)...};
        }

        // attach qt updater methods to the value
        template <typename U, typename ...Args>
        auto attach_qt_methods(U qt_object, Args&&... methods) -> void
        {
            // attach the qt methods
            m_qt_methods.is = true;
            m_qt_methods.methods = std::set{std::bind_front(std::forward<Args>(methods), qt_object)...};
        }

    private:
        // underlying value and lock control
        T m_value;
        bool m_locked = true;

        // clamp and constraint information
        struct {bool is = false; std::pair<int, int> range  ;} m_clamp;
        struct {bool is = false; std::set<T>         values ;} m_constraints;
        struct {bool is = false; qt_method_set_t       methods;} m_qt_methods;

        auto check_filters(T that) -> std::tuple<bool, T>
        {
            // constraint
            if (m_constraints.is and not m_constraints.values.contains(that)) return std::tuple(false, that);
            return std::tuple(true, that);
        }

        auto check_filters(T that) -> std::tuple<bool, T> requires (std::is_arithmetic_v<T> and not std::is_same_v<T, bool>)
        {
            // constraint and clamp
            if (m_constraints.is and not m_constraints.values.contains(that)) return std::tuple(false, that);
            if (m_clamp.is) return std::tuple(true, std::clamp(that, m_clamp.range.first, m_clamp.range.second));
            return std::tuple(true, that);
        }
    };

public:
    using value_type = T;

public:
    // constructors
    property() = default;
    property(const T& that) {_meta.m_value = that;}
    property(T&& that) noexcept {_meta.m_value = std::forward<T>(that);}
    ~property() {_meta.deleter();}

    property(const property&) = default;
    property(property&&) noexcept = default;
    auto operator=(const property&) -> property& = default;
    auto operator=(property&&) noexcept -> property& = default;

    // member access
    auto operator* () -> T& {VERIFY_LOCK return _meta.m_value;}
    auto operator->() const -> auto requires (is_smart_ptr_v<T>) {return _meta.getter().get();}
    auto operator->() const -> T  requires (std::is_pointer_v<T>) {return _meta.getter();}
    auto operator->() const -> T* requires (!std::is_pointer_v<T> && !is_smart_ptr_v<T>) {return &_meta.getter();}

    // getter
    operator T() const requires (!is_smart_ptr_v<T>) {return _meta.getter();}

    template <typename U> operator U() const requires (is_smart_ptr_v<T>) {return _meta.getter().get();}

    // setter
    auto operator=(T&& that) -> property&
    {
        auto&& [pass, value] = _meta.check_filters(that);
        if (pass) _meta.setter(value);
        for (const auto& qt_method: _meta.m_qt_methods.methods) qt_method(value);
        return *this;
    }

    auto operator=(const T& that) -> property& {operator=(std::move(that));}

    // comparison operators
    template <typename U> auto operator==(const U& that) -> bool {return _meta.m_value == that;}
    template <typename U> auto operator!=(const U& that) -> bool {return _meta.m_value != that;}
    template <typename U> auto operator> (const U& that) -> bool {return _meta.m_value >  that;}
    template <typename U> auto operator< (const U& that) -> bool {return _meta.m_value <  that;}
    template <typename U> auto operator>=(const U& that) -> bool {return _meta.m_value >= that;}
    template <typename U> auto operator<=(const U& that) -> bool {return _meta.m_value <= that;}

    // assignment operators
    template <typename U> auto operator+=(const U& that) -> property& {_meta.m_value += that; return *this;}
    template <typename U> auto operator-=(const U& that) -> property& {_meta.m_value -= that; return *this;}
    template <typename U> auto operator*=(const U& that) -> property& {_meta.m_value *= that; return *this;}
    template <typename U> auto operator/=(const U& that) -> property& {_meta.m_value /= that; return *this;}
    template <typename U> auto operator%=(const U& that) -> property& {_meta.m_value %= that; return *this;}

    // arithmetic operators
    template <typename U> auto operator+(const U& that) const -> property {return property{_meta.m_value += that};}
    template <typename U> auto operator-(const U& that) const -> property {return property{_meta.m_value -= that};}
    template <typename U> auto operator*(const U& that) const -> property {return property{_meta.m_value *= that};}
    template <typename U> auto operator/(const U& that) const -> property {return property{_meta.m_value /= that};}
    template <typename U> auto operator%(const U& that) const -> property {return property{_meta.m_value %= that};}

    // increment operators
    template <typename U> auto operator++() -> property& {++_meta.m_value; return *this;}
    template <typename U> auto operator--() -> property& {--_meta.m_value; return *this;}
    template <typename U> auto operator++(const int) const -> property {return property{_meta.m_value++};}
    template <typename U> auto operator--(const int) const -> property {return property{_meta.m_value--};}

    // boolean operator
    operator bool() const requires (not std::is_same_v<T, bool>) {return static_cast<bool>(_meta.getter());}

    meta _meta;
};


template <typename U, typename T>
auto p_static_cast(
        const ext::property<T>& that)
        -> ext::property<U>
{
    return property<U>(dynamic_cast<U>(*that));
}


template <typename U, typename T>
auto p_dynamic_cast(
        const ext::property<T>& that)
        -> ext::property<U>
{
    return property<U>(static_cast<U>(*that));
}


template <typename U, typename T>
auto p_const_cast(
        const ext::property<T>& that)
        -> ext::property<U>
{
    return property<U>(const_cast<U>(*that));
}


template <typename U, typename T>
auto p_reinterpret_cast(
        const ext::property<T>& that)
        -> ext::property<U>
{
    return property<U>(reinterpret_cast<U>(*that));
}


template <typename U, typename T>
auto p_any_cast(
        const ext::property<T>& that)
        -> ext::property<U>
{
    return property<U>(std::any_cast<U>(*that));
}


template <typename U, typename T>
auto p_bit_cast(
        const ext::property<T>& that)
        -> ext::property<U>
{
    return property<U>(std::bit_cast<U>(*that));
}


#endif //SBROWSER_PROPERTY_HPP
