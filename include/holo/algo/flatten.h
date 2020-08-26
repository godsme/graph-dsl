//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_FLATTEN_H
#define GRAPH_FLATTEN_H

#include <holo/algo/head.h>
#include <holo/algo/partition.h>
#include <holo/algo/concat.h>

HOLO_NS_BEGIN

namespace {
   template <typename T>
   struct flatten_helper {
      constexpr flatten_helper(T value) : value_(value) {}
      T value_;
   };

   template<typename T, typename ELEM>
   constexpr auto operator+(flatten_helper<T> acc, ELEM elem) {
      return flatten_helper{std::tuple_cat(acc.value_, elem)};
   }
}

struct flatten_class final {
   template<typename ... Ts>
   constexpr static auto flatten_1(const std::tuple<Ts...>& list) {
      return flatten(list);
   }

   template<typename T>
   constexpr static auto flatten_1(const T& value) {
      return std::make_tuple(value);
   }

   template<size_t ... I, typename LIST>
   constexpr static auto flatten(const LIST& list, std::index_sequence<I...>) {
      return (flatten_helper{tuple_t<>} + ... + flatten_1(std::get<I>(list))).value_;
   }

   template<typename LIST>
   constexpr static auto flatten(const LIST& list) {
      return flatten(list, std::make_index_sequence<std::tuple_size_v<LIST>>{});
   }
};

template<typename LIST>
constexpr auto flatten(const LIST& list) {
   return flatten_class::flatten(list);
}

HOLO_NS_END

#endif //GRAPH_FLATTEN_H
