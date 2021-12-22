#pragma once
#ifndef SBROWSER_PROPERTY_HPP
#define SBROWSER_PROPERTY_HPP

#include <functional>
#include <ext/any.hpp>
#include <ext/string.hpp>
#include <ext/vector.hpp>

#define readonly_setter [](T) {}


namespace ext {
    template <typename T> class property;
    template <typename T> class dom_property;
    class css_property;
    class css_shorthand_property;

    template <typename U, typename T> property<U> property_dynamic_cast(const ext::property<T>& other);
    template <typename U, typename T> property<U> property_static_cast(const ext::property<T>& other);
    template <typename U, typename T> property<U> property_const_cast(const ext::property<T>& other);
    template <typename U, typename T> property<U> property_reinterpret_cast(const ext::property<T>& other);
}


namespace {
    template<typename T>
    void ce_reactions(const ext::dom_property<T>& property) {/* TODO */};

    template<typename function_t>
    void ce_reactions(const function_t& function) requires(std::is_member_function_pointer_v<function_t>) {/* TODO */};
}


template <typename T>
class ext::property {
    // default setters and getters for const-references (override custom getter / setter)
    friend T            __fastcall operator >> (const property<T>& property, T& other) {other = property.m_value; return other;}     // get
    friend property<T>& __fastcall operator << (property<T>& property, const T& other) {property.m_value = other; return property;}; // set

    // default setters and getters for temp-objects (override custom getter / setter)
    friend T            __fastcall operator >> (const property<T>& property, T&& other) {other = property.m_value; return other;};    // get
    friend property<T>& __fastcall operator << (property<T>& property, const T&& other) {property.m_value = other; return property;}; // set

    // property casting helpers methods (friend access for internal value)
    template <typename U, typename T> friend property<U> ext::property_dynamic_cast(const ext::property<T>& other);
    template <typename U, typename T> friend property<U> ext::property_static_cast(const ext::property<T>& other);
    template <typename U, typename T> friend property<U> ext::property_const_cast(const ext::property<T>& other);
    template <typename U, typename T> friend property<U> ext::property_reinterpret_cast(const ext::property<T>& other);

public:
    // main constructor to assign the deleter, getter and setter
    property() {
        del = []() -> void {};
        get = [this]() -> T {return m_value;};
        set = [this](T val) -> void {m_value = val;};
    }

    // copy constructor to copy properties between objects
    property(const property<T>& other) = default;

    // deleter, getter and setter calling at the correct places
    virtual ~property() {del();}
    __forceinline virtual __fastcall operator T() const {return get();}
    __forceinline virtual void __fastcall operator=(const T& val) {set(val);}

    // use pointer operand to access the attributes of the internal value
    __forceinline T* __fastcall operator->() requires (not std::is_pointer_v<T>) {return &get();}
    __forceinline T  __fastcall operator->() requires (std::is_pointer_v<T>) {return get();}

    // boolean comparison operators against another value (property will auto-cast into T)
    __forceinline bool __fastcall operator== (const T& other) const {return m_value ==  other;}
    __forceinline bool __fastcall operator!= (const T& other) const {return m_value !=  other;}
    __forceinline bool __fastcall operator<= (const T& other) const {return m_value <=  other;}
    __forceinline bool __fastcall operator>= (const T& other) const {return m_value >=  other;}
    __forceinline bool __fastcall operator<  (const T& other) const {return m_value <   other;}
    __forceinline bool __fastcall operator>  (const T& other) const {return m_value >   other;}
    __forceinline bool __fastcall operator<=>(const T& other) const {return m_value <=> other;}

    // create-arithmetic operators against another value
    __forceinline property<T> __fastcall operator+(const T& other) const {return property<T>{m_value + other};}
    __forceinline property<T> __fastcall operator-(const T& other) const {return property<T>{m_value - other};}
    __forceinline property<T> __fastcall operator*(const T& other) const {return property<T>{m_value * other};}
    __forceinline property<T> __fastcall operator/(const T& other) const {return property<T>{m_value / other};}
    __forceinline property<T> __fastcall operator%(const T& other) const {return property<T>{m_value % other};}
    __forceinline property<T> __fastcall operator^(const T& other) const {return property<T>{m_value & other};}
    __forceinline bool __fastcall operator&(const T& other) const {return m_value & other;}
    __forceinline bool __fastcall operator|(const T& other) const {return m_value | other;}

    // modification-arithmetic operators against another value
    __forceinline property<T>& __fastcall operator+=(const T& other) {m_value += other; return *this;}
    __forceinline property<T>& __fastcall operator-=(const T& other) {m_value -= other; return *this;}
    __forceinline property<T>& __fastcall operator*=(const T& other) {m_value *= other; return *this;}
    __forceinline property<T>& __fastcall operator/=(const T& other) {m_value /= other; return *this;}
    __forceinline property<T>& __fastcall operator%=(const T& other) {m_value %= other; return *this;}
    __forceinline property<T>& __fastcall operator^=(const T& other) {m_value ^= other; return *this;}
    __forceinline property<T>& __fastcall operator&=(const T& other) {m_value &= other; return *this;}
    __forceinline property<T>& __fastcall operator|=(const T& other) {m_value |= other; return *this;}

    __forceinline property<T>& __fastcall operator++() {++m_value; return *this;}
    __forceinline property<T>& __fastcall operator--() {--m_value; return *this;}

    __forceinline property<T>  __fastcall operator++(const int n) const {return property<T>{m_value + n};}
    __forceinline property<T>  __fastcall operator--(const int n) const {return property<T>{m_value - n};}

    __forceinline property<T>& __fastcall operator-() {m_value.operator-(); return *this;}
    __forceinline property<T>& __fastcall operator+() {m_value.operator+(); return *this;}

    __forceinline property<T> __fastcall operator<<(const std::size_t n) const {return property<T>{m_value << n};}
    __forceinline property<T> __fastcall operator>>(const std::size_t n) const {return property<T>{m_value >> n};}

    __forceinline property<T>& __fastcall operator<<=(const std::size_t n) {m_value <<= n; return *this;}
    __forceinline property<T>& __fastcall operator>>=(const std::size_t n) {m_value >>= n; return *this;}

    __forceinline property<T> __fastcall operator&&(const T& other) const {return property<T>{m_value && other};}
    __forceinline property<T> __fastcall operator||(const T& other) const {return property<T>{m_value || other};}

    __forceinline T __fastcall operator~() const {return ~m_value;}
    __forceinline bool __fastcall operator!() const {return not m_value;}

    __forceinline T __fastcall operator=(const property<T>& other) {m_value = other.m_value; return *this;}
    __forceinline T __fastcall operator=(property<T>&& other) noexcept {m_value = other.m_value; return *this;}

    __forceinline std::remove_reference_t<T> __fastcall operator*() const {return *m_value;}

    __forceinline bool __fastcall operator[] (const T index) const {return m_value[index];}
    __forceinline bool __fastcall operator() () const {return m_value();}

    __forceinline std::remove_pointer_t<T> __fastcall operator*() const requires (std::is_pointer_v<T>) {return *m_value;}

    __forceinline operator bool() requires (not std::is_same_v<T, bool>) {return std::is_same_v<T, std::any> ? ext::any_cast<bool>(m_value) : (bool)m_value;}

protected:
    std::function<void __fastcall ( )> del;
    std::function<T    __fastcall ( )> get;
    std::function<void __fastcall (T)> set;

private:
    T m_value;
};


template <typename T>
class ext::dom_property : public ext::property<T> {
public:
    explicit dom_property(bool ce_reactions = false)
            : ext::property<T>()
            , m_ce_reactions(ce_reactions) {
    }

    dom_property(const dom_property<T>&) = default;

    ~dom_property() override {
        if (m_ce_reactions) ce_reactions(*this);
        this->del();
    }

    __forceinline __fastcall operator T() const override {
        if (m_ce_reactions) ce_reactions(*this);
        return this->get();
    }

    __forceinline void __fastcall operator=(T val) {
        if (m_ce_reactions) ce_reactions(*this);
        this->set(val);
    }

private:
    bool m_ce_reactions;
};


class ext::css_property : public ext::property<string> {
    friend class ext::css_shorthand_property;

    enum animation_types {not_animatable, by_computed_value, discrete, repeatable_list, number, list};
    enum percentage_types {na, corresponding_dimension_content_area, logical_width};
    enum canonical_order_types {per_grammar};
    enum media_types {visual, hidden};

public:
    css_property(
            cstring& name, cstring& initial, bool inherited = false,
            animation_types animation_type = not_animatable, percentage_types percentage_type = na,
            canonical_order_types canonical_order_type = per_grammar, media_types media_type = hidden)

            : ext::property<string>()
            , m_name(name)
            , m_initial(initial)
            , m_inherited(inherited)
            , m_animation_type(animation_type)
            , m_percentage_type(percentage_type)
            , m_canonical_order_type(canonical_order_type)
            , m_media_type(media_type) {
    }

private:
    string m_name;
    string m_initial;
    bool m_inherited;
    animation_types m_animation_type;
    percentage_types m_percentage_type;
    canonical_order_types m_canonical_order_type;
    media_types m_media_type;
};


class ext::css_shorthand_property : public ext::property<string> {
//    friend css_property& __fastcall operator << (css_shorthand_property& shorthand_property, string other) {
//        auto sub_items = shorthand_property.extract_sub_items(other);
//        shorthand_property.m_longhand_properties.for_each([&sub_items](css_property* longhand_property) -> void {*longhand_property << sub_items.front(); sub_items.pop(0);});
//    };

public:
    css_shorthand_property(cstring& name)
            : ext::property<string>()
            , m_name(name) {
    }

    __forceinline css_shorthand_property& assign_longhand_property(css_property longhand_property) {
        m_longhand_properties.append(&longhand_property);
        m_initial += (string)" " + longhand_property.m_initial;
        return *this;
    }

    __forceinline __fastcall operator string() const override {
        return m_longhand_properties
                .transform<string>([](css_property* longhand_property) -> string {return longhand_property->get();})
                .join(" ");
    }

    __forceinline void __fastcall operator=(cstring& val) override {
        auto sub_items = extract_sub_items(val);
        m_longhand_properties.for_each([&sub_items](css_property* longhand_property) -> void {longhand_property->set(sub_items.front()); sub_items.pop(0);});
    }

private:
    string m_name;
    string m_initial;
    vector<css_property*> m_longhand_properties;

    vector<string> extract_sub_items(string val) {
        auto sub_items = val.split(" ");
        return sub_items.length() < m_longhand_properties.length()
                ? vector<string>{sub_items.front()} * m_longhand_properties.length()
                : sub_items;
    }
};


template <typename U, typename T>
ext::property<U> ext::property_static_cast(const property<T>& other) {
    ext::property<U> casted_property{};
    casted_property << static_cast<U>(other.m_value);
    casted_property.del = other.del;
    casted_property.get = other.get;
    casted_property.set = other.set;
    return casted_property;
}


template <typename U, typename T>
ext::property<U> ext::property_dynamic_cast(const property<T>& other) {
    ext::property<U> casted_property{};
    casted_property << dynamic_cast<U>(other.m_value);
    casted_property.del = other.del;
    casted_property.get = other.get;
    casted_property.set = other.set;
    return casted_property;
}


template <typename U, typename T>
ext::property<U> ext::property_const_cast(const property<T>& other) {
    ext::property<U> casted_property{};
    casted_property << const_cast<U>(other.m_value);
    casted_property.del = other.del;
    casted_property.get = other.get;
    casted_property.set = other.set;
    return casted_property;
}


template <typename U, typename T>
ext::property<U> ext::property_reinterpret_cast(const property<T>& other) {
    ext::property<U> casted_property{};
    casted_property << reinterpret_cast<U>(other.m_value);
    casted_property.del = other.del;
    casted_property.get = other.get;
    casted_property.set = other.set;
    return casted_property;
}


#endif //SBROWSER_PROPERTY_HPP
