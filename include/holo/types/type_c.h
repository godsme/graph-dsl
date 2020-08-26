//
// Created by Darwin Yuan on 2020/8/17.
//

#ifndef GRAPH_TYPE_C_H
#define GRAPH_TYPE_C_H

#include <holo/types/integral_c.h>
#include <type_traits>

HOLO_NS_BEGIN

template <typename T>
struct type_c_t {
   constexpr type_c_t() noexcept = default;
   constexpr type_c_t(const type_c_t&) noexcept = default;
   constexpr type_c_t(type_c_t&&) noexcept = default;
   using type = T;
};

template <typename U, typename V>
inline constexpr auto operator==(type_c_t<U> const& lhs, type_c_t<V> const& rhs) noexcept {
   return integral_c<bool, std::is_same_v<U, V>>{};
}

template <typename U, typename V>
inline constexpr auto operator!=(type_c_t<U> const& lhs, type_c_t<V> const& rhs) noexcept {
   return integral_c<bool, !std::is_same_v<U, V>>{};
}

template <typename T>
constexpr type_c_t<T> type_c{};

HOLO_NS_END

#endif //GRAPH_TYPE_C_H
