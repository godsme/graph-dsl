//
// Created by Darwin Yuan on 2020/8/17.
//

#include <catch.hpp>
#include <holo/algo/fold_left.h>
#include <holo/types/tuple_t.h>
#include <optional>
#include <iostream>
#include <holo/algo/find_if.h>
#include <holo/algo/transform.h>
#include <holo/algo/filter.h>
#include <holo/algo/contains.h>
#include <holo/algo/append.h>
#include <holo/algo/remove_if.h>

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
   struct value_holder { constexpr static auto value = VALUE; };

   TEST_CASE("holo fold left 1") {
      constexpr auto result = holo::fold_left(0,
          [](auto const& acc, auto elem){
            return decltype(elem)::type::value + acc;
          },
          holo::tuple_t<value_holder<1>, value_holder<2>, value_holder<3>>);

      REQUIRE(result == 6);
   }

   TEST_CASE("holo find if") {
      constexpr auto result = holo::find_if(
         [](auto elem) constexpr {
            return elem == holo::type_c<double>;
         },
         holo::tuple_t<int, double, float>);

      static_assert(result.has_value(), "");
      static_assert(*result == std::optional{holo::type_c<double>});
   }

   template <typename T> struct w{};
   TEST_CASE("holo transform") {
      constexpr auto result = holo::transform(
         [](auto elem) constexpr {
            return holo::type_c<w<typename decltype(elem)::type>>;
         },
         holo::tuple_t<int, double, float>);

      static_assert(result == holo::tuple_t<w<int>, w<double>, w<float>>);
   }

   TEST_CASE("holo filter") {
      constexpr auto result = holo::filter(
         [](auto elem) constexpr {
            return elem != holo::type_c<double>;
         },
         holo::tuple_t<int, double, float>);

      static_assert(result == holo::tuple_t<int, float>);
   }

   TEST_CASE("holo remove_if") {
      constexpr auto result = holo::remove_if(
         [](auto elem) constexpr {
            return elem == holo::type_c<double>;
         },
         holo::tuple_t<int, double, float>);

      static_assert(result == holo::tuple_t<int, float>);
   }

   TEST_CASE("std constexpr test") {
      constexpr std::optional<int> i = 10;
      static_assert(*i == 10, "");
      static_assert(i.has_value(), "");
      
      constexpr auto pair = std::pair{holo::type_c<int>, holo::type_c<double>};
      static_assert(pair == std::make_pair(holo::type_c<int>, holo::type_c<double>));
   }

   TEST_CASE("contains") {
      static_assert(holo::contains(holo::type_c<int>, holo::tuple_t<int, double, float>));
      static_assert(!holo::contains(holo::type_c<long>, holo::tuple_t<int, double, float>));
   }

   TEST_CASE("append") {
      static_assert(holo::tuple_t<int, double, float, long> == holo::append(holo::type_c<long>, holo::tuple_t<int, double, float>));
      static_assert(holo::tuple_t<long, int, double, float> == holo::prepend(holo::type_c<long>, holo::tuple_t<int, double, float>));
      static_assert(holo::tuple_t<long, char, int, double, float> == holo::concat(holo::tuple_t<long, char>, holo::tuple_t<int, double, float>));
   }
}