//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_SIZEOF_C_H
#define GRAPH_SIZEOF_C_H

#include <holo/holo_ns.h>
#include <type_traits>

HOLO_NS_BEGIN

template<typename T>
struct sizeof_const {
};

template<typename T1, typename T2>
inline constexpr auto operator==(sizeof_const<T1> const& lhs, sizeof_const<T2> const& rhs) {
   return std::integral_constant<bool, sizeof(T1) == sizeof(T2)>{};
}

template<typename T1, typename T2>
inline constexpr auto operator!=(sizeof_const<T1> const& lhs, sizeof_const<T2> const& rhs) {
   return std::integral_constant<bool, sizeof(T1) != sizeof(T2)>{};
}

template<typename T1, typename T2>
inline constexpr auto operator<(sizeof_const<T1> const& lhs, sizeof_const<T2> const& rhs) {
   return std::integral_constant<bool, sizeof(T1) < sizeof(T2)>{};
}

template<typename T1, typename T2>
inline constexpr auto operator<=(sizeof_const<T1> const& lhs, sizeof_const<T2> const& rhs) {
   return std::integral_constant<bool, sizeof(T1) <= sizeof(T2)>{};
}

template<typename T1, typename T2>
inline constexpr auto operator>(sizeof_const<T1> const& lhs, sizeof_const<T2> const& rhs) {
   return std::integral_constant<bool, (sizeof(T1) > sizeof(T2))>{};
}

template<typename T1, typename T2>
inline constexpr auto operator>=(sizeof_const<T1> const& lhs, sizeof_const<T2> const& rhs) {
   return std::integral_constant<bool, (sizeof(T1) >= sizeof(T2))>{};
}

template<typename T>
constexpr sizeof_const<T> sizeof_c{};

HOLO_NS_END

#endif //GRAPH_SIZEOF_C_H
