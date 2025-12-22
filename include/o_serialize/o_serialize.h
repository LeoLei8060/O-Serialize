#ifndef O_SERIALIZE_H
#define O_SERIALIZE_H

#include <tuple>
#include <functional>
#include <type_traits>

namespace OSerialize {

namespace Meta {

    template <typename T>
    struct Reflector {
        static constexpr bool is_defined = false;
    };

    // Helper to check if a type has reflection defined
    template <typename T>
    struct has_reflection {
        static constexpr bool value = Reflector<T>::is_defined;
    };

    // Function to apply a visitor to all members
    template <typename T, typename Visitor>
    typename std::enable_if<has_reflection<T>::value>::type
    visit_members(T& obj, Visitor&& visitor) {
        Reflector<T>::visit(obj, std::forward<Visitor>(visitor));
    }

    template <typename T, typename Visitor>
    typename std::enable_if<has_reflection<T>::value>::type
    visit_members(const T& obj, Visitor&& visitor) {
        Reflector<T>::visit(obj, std::forward<Visitor>(visitor));
    }

} // namespace Meta

} // namespace OSerialize

// Helper macros
#define EXPAND(x) x
#define GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME

#define FE_1(action, x) action(x)
#define FE_2(action, x, ...) action(x) FE_1(action, __VA_ARGS__)
#define FE_3(action, x, ...) action(x) FE_2(action, __VA_ARGS__)
#define FE_4(action, x, ...) action(x) FE_3(action, __VA_ARGS__)
#define FE_5(action, x, ...) action(x) FE_4(action, __VA_ARGS__)
#define FE_6(action, x, ...) action(x) FE_5(action, __VA_ARGS__)
#define FE_7(action, x, ...) action(x) FE_6(action, __VA_ARGS__)
#define FE_8(action, x, ...) action(x) FE_7(action, __VA_ARGS__)
#define FE_9(action, x, ...) action(x) FE_8(action, __VA_ARGS__)
#define FE_10(action, x, ...) action(x) FE_9(action, __VA_ARGS__)

#define FOR_EACH(action, ...) \
    GET_MACRO(__VA_ARGS__, FE_10, FE_9, FE_8, FE_7, FE_6, FE_5, FE_4, FE_3, FE_2, FE_1)(action, __VA_ARGS__)

// The action will be: visitor(#member, obj.member);
#define VISIT_MEMBER(member) visitor(#member, obj.member);

// Macro to register a struct
#define O_SERIALIZE_STRUCT(Type, ...) \
namespace OSerialize { namespace Meta { \
    template <> \
    struct Reflector<Type> { \
        static constexpr bool is_defined = true; \
        \
        template <typename Visitor> \
        static void visit(Type& obj, Visitor&& visitor) { \
            FOR_EACH(VISIT_MEMBER, __VA_ARGS__) \
        } \
        \
        template <typename Visitor> \
        static void visit(const Type& obj, Visitor&& visitor) { \
            FOR_EACH(VISIT_MEMBER, __VA_ARGS__) \
        } \
    }; \
}}

#endif // O_SERIALIZE_H
