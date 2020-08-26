//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_BOOL_C_H
#define GRAPH_BOOL_C_H

#include <holo/holo_ns.h>
#include <type_traits>

HOLO_NS_BEGIN

template<bool V>
struct bool_const {
   constexpr operator bool() const {
      return V;
   }
};

template<bool V1, bool V2>
inline constexpr auto operator==(bool_const<V1> const& lhs, bool_const<V2> const& rhs) {
   return std::integral_constant<bool, V1 == V2>{};
}

template<bool V1>
inline constexpr auto operator==(bool_const<V1> const& lhs, bool rhs) {
   return std::integral_constant<bool, V1 == rhs>{};
}

template<bool V2>
inline constexpr auto operator==(bool lhs, bool_const<V2> const& rhs) {
   return std::integral_constant<bool, lhs == V2>{};
}

template<bool V1, bool V2>
inline constexpr auto operator!=(bool_const<V1> const& lhs, bool_const<V2> const& rhs) {
   return std::integral_constant<bool, V1 != V2>{};
}

template<bool V1>
inline constexpr auto operator!=(bool_const<V1> const& lhs, bool rhs) {
   return std::integral_constant<bool, V1 != rhs>{};
}

template<bool V2>
inline constexpr auto operator!=(bool lhs, bool_const<V2> const& rhs) {
   return std::integral_constant<bool, lhs != V2>{};
}

template<bool B>
constexpr bool_const<B> bool_c{};

HOLO_NS_END

#endif //GRAPH_BOOL_C_H
