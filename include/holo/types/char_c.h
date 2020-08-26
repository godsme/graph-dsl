//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_CHAR_C_H
#define GRAPH_CHAR_C_H

#include <holo/holo_ns.h>
#include <type_traits>

HOLO_NS_BEGIN

template<char V>
struct char_const {
   constexpr operator char() const {
      return V;
   }
};

template<char V1, char V2>
inline constexpr auto operator==(char_const<V1> const& lhs, char_const<V2> const& rhs) {
   return std::integral_constant<bool, V1 == V2>{};
}

template<char V1, char V2>
inline constexpr auto operator!=(char_const<V1> const& lhs, char_const<V2> const& rhs) {
   return std::integral_constant<bool, V1 != V2>{};
}

template<char V1, char V2>
inline constexpr auto operator<(char_const<V1> const& lhs, char_const<V2> const& rhs) {
   return std::integral_constant<bool, V1 < V2>{};
}

template<char V1, char V2>
inline constexpr auto operator<=(char_const<V1> const& lhs, char_const<V2> const& rhs) {
   return std::integral_constant<bool, V1 <= V2>{};
}

template<char V1, char V2>
inline constexpr auto operator>(char_const<V1> const& lhs, char_const<V2> const& rhs) {
   return std::integral_constant<bool, (V1 > V2)>{};
}

template<char V1, char V2>
inline constexpr auto operator>=(char_const<V1> const& lhs, char_const<V2> const& rhs) {
   return std::integral_constant<bool, (V1 >= V2)>{};
}

template <char V>
constexpr char_const<V> char_c{};

HOLO_NS_END

#endif //GRAPH_CHAR_C_H
