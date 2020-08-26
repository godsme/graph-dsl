//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_SIZE_C_H
#define GRAPH_SIZE_C_H

#include <holo/holo_ns.h>
#include <type_traits>

HOLO_NS_BEGIN

template<size_t V>
struct size_const {
   constexpr operator size_t() const {
      return V;
   }
};

template<size_t V1, size_t V2>
inline constexpr auto operator==(size_const<V1> const& lhs, size_const<V2> const& rhs) {
   return std::integral_constant<bool, V1 == V2>{};
}

template<size_t V1, size_t V2>
inline constexpr auto operator!=(size_const<V1> const& lhs, size_const<V2> const& rhs) {
   return std::integral_constant<bool, V1 != V2>{};
}

template<size_t V1, size_t V2>
inline constexpr auto operator<(size_const<V1> const& lhs, size_const<V2> const& rhs) {
   return std::integral_constant<bool, V1 < V2>{};
}

template<size_t V1, size_t V2>
inline constexpr auto operator<=(size_const<V1> const& lhs, size_const<V2> const& rhs) {
   return std::integral_constant<bool, V1 <= V2>{};
}

template<size_t V1, size_t V2>
inline constexpr auto operator>(size_const<V1> const& lhs, size_const<V2> const& rhs) {
   return std::integral_constant<bool, (V1 > V2)>{};
}

template<size_t V1, size_t V2>
inline constexpr auto operator>=(size_const<V1> const& lhs, size_const<V2> const& rhs) {
   return std::integral_constant<bool, (V1 >= V2)>{};
}

template <size_t V>
constexpr size_const<V> size_c{};

HOLO_NS_END

#endif //GRAPH_SIZE_C_H
