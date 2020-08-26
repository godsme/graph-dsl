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
#include <holo/algo/concat.h>
#include <holo/algo/remove_if.h>
#include <holo/algo/reverse.h>
#include <holo/algo/head.h>
#include <holo/algo/partition.h>
#include <holo/algo/sort.h>
#include <holo/types/sizeof_c.h>

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

   TEST_CASE("reverse") {
      static_assert(holo::tuple_t<float, double, int> == holo::reverse(holo::tuple_t<int, double, float>));
   }

   TEST_CASE("head") {
      static_assert(holo::type_c<int> == holo::head(holo::tuple_t<int, double, float>));
      static_assert(holo::tuple_t<double, float> == holo::tail(holo::tuple_t<int, double, float>));
   }

   template <typename T> struct S;
   TEST_CASE("partition") {
      constexpr auto result = holo::partition(
         [](auto elem) constexpr {
            return holo::sizeof_c<typename std::decay_t<decltype(elem)>::type> < holo::sizeof_c<size_t>;
         },
         holo::tuple_t<int, long long, char, float, short, double, bool, long double>);

      static_assert(result == std::make_pair(holo::tuple_t<int, char, float, short, bool>, holo::tuple_t<long long, double, long double>));
   }

   template <typename T> struct S;
   TEST_CASE("sort") {
      constexpr auto result = holo::sort(
         [](auto l, auto r) constexpr {
            return holo::sizeof_c<typename std::decay_t<decltype(l)>::type> <
            holo::sizeof_c<typename std::decay_t<decltype(r)>::type>;
         },
         holo::tuple_t<int, long long, short, char>);

      static_assert(result == holo::tuple_t<char, short, int, long long>);
   }
}