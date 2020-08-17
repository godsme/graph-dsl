//
// Created by Darwin Yuan on 2020/8/18.
//

#ifndef GRAPH_FIND_IF_H
#define GRAPH_FIND_IF_H

#include <holo/holo_ns.h>
#include <holo/types/type_c.h>
#include <type_traits>
#include <tuple>
#include <optional>

HOLO_NS_BEGIN

namespace detail {
   template<size_t I, typename TUPLE, typename F>
   constexpr auto find_if(F&& f, TUPLE&& tuple) {
      if constexpr (I == std::tuple_size_v<std::decay_t<TUPLE>>) {
         return std::nullopt;
      }
      else if constexpr (std::is_same_v<decltype(f(std::get<I>(tuple))), std::integral_constant<bool, true>>) {
         return std::optional{std::get<I>(tuple)};
      }
      else {
         return find_if<I+1>(std::forward<F>(f), std::forward<TUPLE>(tuple));
      }
   }
}

template<typename TUPLE, typename F>
constexpr auto find_if(F&& f, TUPLE&& tuple) {
   return detail::find_if<0>(std::forward<F>(f), std::forward<TUPLE>(tuple));
}

HOLO_NS_END

#endif //GRAPH_FIND_IF_H
