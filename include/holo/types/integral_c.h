//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_INTEGRAL_C_H
#define GRAPH_INTEGRAL_C_H

#include <holo/holo_ns.h>
#include <type_traits>

HOLO_NS_BEGIN

template<typename T, T V>
struct integral_c {
   constexpr operator T() const { return V; }
};

template<typename T, T V1, T V2>
inline constexpr auto operator==(integral_c<T, V1> const& lhs, integral_c<T, V2> const& rhs) {
   return integral_c<bool, V1 == V2>{};
}

template<typename T, T V1, T V2>
inline constexpr auto operator!=(integral_c<T, V1> const& lhs, integral_c<T, V2> const& rhs) {
   return integral_c<bool, V1 != V2>{};
}

template<typename T, T V1, T V2>
inline constexpr auto operator<(integral_c<T, V1> const& lhs, integral_c<T, V2> const& rhs) {
   return integral_c<bool, V1 < V2>{};
}

template<typename T, T V1, T V2>
inline constexpr auto operator<=(integral_c<T, V1> const& lhs, integral_c<T, V2> const& rhs) {
   return integral_c<bool, V1 <= V2>{};
}

template<typename T, T V1, T V2>
inline constexpr auto operator>(integral_c<T, V1> const& lhs, integral_c<T, V2> const& rhs) {
   return integral_c<bool, (V1 > V2)>{};
}

template<typename T, T V1, T V2>
inline constexpr auto operator>=(integral_c<T, V1> const& lhs, integral_c<T, V2> const& rhs) {
   return integral_c<bool, (V1 >= V2)>{};
}

HOLO_NS_END

#endif //GRAPH_INTEGRAL_C_H
