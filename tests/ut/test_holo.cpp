//
// Created by Darwin Yuan on 2020/8/17.
//

#include <catch.hpp>
#include <holo/algo/fold_left.h>
#include <holo/types/tuple_t.h>
#include <iostream>

namespace {
   TEST_CASE("holo fold left") {
      constexpr auto result = holo::fold_left(holo::tuple_t<>,
         [](auto const& acc, auto const& elem){
            return std::tuple_cat(acc, std::tuple<std::decay_t<decltype(elem)>>{});
         },
         holo::tuple_t<int, float, double>);

      static_assert(result == holo::tuple_t<int, float, double>);
   }

   template<int VALUE>
   struct value_holder {
      constexpr static auto value = VALUE;
   };
   TEST_CASE("holo fold left 1") {
      constexpr auto result = holo::fold_left(0,
          [](auto const& acc, auto elem){
            return decltype(elem)::type::value + acc;
          },
          holo::tuple_t<value_holder<1>, value_holder<2>, value_holder<3>>);

      static_assert(result == 6);
   }

}