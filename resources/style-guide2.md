##Namespaces:
```cpp
namespace underscore_case {...};
namespace underscore_case::continued {...}
```

##Classes
```cpp
class underscore_case {...};
class underscore_case::continued {...};
```

```cpp
// General c++ class

class underscore_case {
public: aliases
    using aliased_type1 = ...;
    using aliased_type2 = ...;
    
public: friends
    friend class underscore_case2;
    friend class underscore_case3;
    friend struct underscore_case4;
    
    friend auto underscore_case5::method1() -> void;
    friend auto underscore_case5::method2() -> void;
    
public: methods
    auto method1() -> void;
    auto method2() -> void;
    
public: propeties
    ext::string property1;
    ext::string property2;
    
public: operators
    operator+=(underscore_case& o) -> underscore_case&;
    operator-=(underscore_case& o) -> underscore_case&;
    
protected ...:
    m_...
    
private ...:
    m_...
};
```

```cpp
// c++ class ready to be exposed to JavaScript for JavaScript

class underscore_case_js {
    public: aliases
        using callback1 = std::function<void(int, int, char)>;
        using callback2 = std::function<void(size_t, size_t)>;
        
    public: friends
        friend class underscore_clas_js2
        friend class underscore_clas_js3
        
    public: constructors
        underscore_case_js = default;
        virtual ~underscore_case_js();
        
    public: operators
        underscore_case_js& operator+=(underscore_case_js& o);
        underscore_case_js& operator-=(underscore_case_js& o);
        
    public: methods
        method1() -> void;
        method2() -> void;
        
    public: properties
        ext::dom_property<ext::string> property1;
        ext::dom_property<ext::string> property2;

    protected: internal_methods
        void m_internal_method1();
        void m_internal_method2();
        
    protected: internal_properties
        ext::string m_property1;
        ext::string m_property2;
        
    protected: accessors
        auto get_property3() const -> ext::string;
        auto get_property4() const -> ext::string;
        auto set_property4(ext::cstring& val) -> void;
        auto set_property5(ext::cstring& val) -> void;

    private: internal_methods
        m_internal_method1() -> void;
        m_internal_method2() -> void;
    
    private: internal_properties
        ext::string m_property1;
        ext::string m_property2;
        
    private: accessors
        auto get_property6() const -> ext::string;
        auto get_property7() const -> ext::string;
        auto set_property6(ext::cstring& val) -> void;
        auto set_property7(ext::cstring& val) -> void;
        
    private: internal_slots
        ext::string s_internal_slot1;
        ext::string s_internal_slot2;

    private: internal_slot_methods
        s_internal_slot_method1() -> void;
        s_internal_slot_method2() -> void;
};
```

##Class methods:
```cpp
// methods with <= 3 lines in the body can have parameters on one line (short types)
underscore_case(type1 param1, ...) -> void
{
    ...
    ...
    ...
}
```

```cpp
// methods with >= 4 lines in the body, or long parameter types and names, have parameters on new lines
underscore_case(
        very_long_type1_pointer very_long_param1_name,
        very_long_type2_pointer very_long_param2_name)
        -> very_long_return_type1_pointer
{
    ...
    ...
    ...
    ...
}
```

##Template classes:
```cpp
template <typename T>
class underscore_case{
public:
    undercore_case() = default;
    method1() const -> ext::string;
    method2() const -> ext::string;
    
private:
    ext::string attribute1;
    ext::string attribute2;
};


template <typename T>
underscore_case::method1() const -> ext::string
{
    ...
    ...
}


template <typename T>
underscore_case::method2() const -> ext::string
{
    ...
    ...
}
```

##Inheritance:
```cpp
class derived_class1 : public base_class1 {
    ...
};


class derived_class2
        : public base_class2
        , public base_class3
        , public base class4 {
    ...
};
```

```cpp
derived_class1::derived_class1() : base_class1()
{
    ...
};


derived_class2::derived_class2()
        : base_class2()
        , base_class3()
        , base_class4() {
    ...
}
```

##Constructors:
```cpp

class class1 {
    class1() = default;
    class1(const class1&) = default;
    class1(class1&&) noexcept = default;
    class1& operator=(const class1&) = default;
    class1& operator=(class1&&) noexcept = default;
    // any other constructors here
    
    virtual ~class1();
};
```

##Forward declaration:
```cpp
class class1;
class class2;

namespace namespace1 {class 3};

namespace namespace2
{
    class 4;
    class 5;
}
```
