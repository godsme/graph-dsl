//
// Created by Darwin Yuan on 2020/8/17.
//

#ifndef GRAPH_FOLD_LEFT_H
#define GRAPH_FOLD_LEFT_H

#include <holo/holo_ns.h>
#include <holo/types/type_c.h>
#include <type_traits>
#include <tuple>

HOLO_NS_BEGIN

namespace detail {
   template <typename T, typename F>
   struct fold_helper {
      constexpr fold_helper(T value, F f) : value_(value), f_(f) {}
      T value_;
      F f_;
   };

   template<typename T, typename F, typename ELEM>
   constexpr auto operator<<(fold_helper<T, F> acc, ELEM elem) {
      return fold_helper(acc.f_(acc.value_, elem), acc.f_);
   }

   template <size_t ... I, typename TUPLE, typename INIT, typename F>
   constexpr auto fold_left(INIT&& init, F&& f, TUPLE&& tuple, std::index_sequence<I...>) {
      auto result = (fold_helper(init, f) <<  ... << std::get<I>(tuple));
      return result.value_;
   }
}

template <typename TUPLE, typename INIT, typename F>
constexpr auto fold_left(INIT&& init, F&& f, TUPLE&& tuple) {
   return detail::fold_left
      ( std::forward<INIT>(init)
      , std::forward<F>(f)
      , std::forward<TUPLE>(tuple),
      std::make_index_sequence<std::tuple_size_v<std::decay_t<TUPLE>>>{});
}

HOLO_NS_END

#endif //GRAPH_FOLD_LEFT_H
