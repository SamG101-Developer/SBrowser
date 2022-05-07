#ifndef SBROWSER_CUSTOM_OPERATOR_HPP
#define SBROWSER_CUSTOM_OPERATOR_HPP


#define define struct
#define binary_operator(ReturnType, OperatorName, FirstOperandType, SecondOperandType)                     \
    OperatorName ## _ {} OperatorName;                                                                     \
    template <typename T>                                                                                  \
    struct OperatorName ## Proxy{                                                                          \
    public:                                                                                                \
        explicit OperatorName ## Proxy(const T& t) : t_(t){}                                               \
        const T& t_;                                                                                       \
        static ReturnType _ ## OperatorName ## _(const FirstOperandType& a, const SecondOperandType& b);   \
    };                                                                                                     \
                                                                                                           \
    template <typename T>                                                                                  \
    OperatorName ## Proxy<T> operator<(const T& lhs, const OperatorName ## _& rhs)                         \
    {                                                                                                      \
        return OperatorName ## Proxy<T>(lhs);                                                              \
    }                                                                                                      \
                                                                                                           \
    ReturnType operator>(const OperatorName ## Proxy<FirstOperandType>& lhs, const SecondOperandType& rhs) \
    {                                                                                                      \
        return OperatorName ## Proxy<FirstOperandType>::_ ## OperatorName ## _(lhs.t_, rhs);               \
    }                                                                                                      \
                                                                                                           \
    template <typename T>                                                                                  \
    inline ReturnType OperatorName ## Proxy<T>::_ ## OperatorName ## _(const FirstOperandType& a, const SecondOperandType& b)


#endif //SBROWSER_CUSTOM_OPERATOR_HPP
