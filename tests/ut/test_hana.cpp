////
//// Created by Darwin Yuan on 2020/8/14.
////
//
//#include <catch.hpp>
//#include <graph/function/unique.h>
//#include <iostream>
//
//namespace {
//   using namespace boost::hana;
//
//   TEST_CASE("hana append") {
//      constexpr auto result = append(make_tuple(type_c<int>, type_c<double>), type_c<long>);
//      static_assert(make_basic_tuple(type_c<int>, type_c<double>, type_c<long>) == result, "");
//   }
//
//   TEST_CASE("hana concat") {
//      constexpr auto result = concat(make_tuple(type_c<int>, type_c<double>),
//                                     make_tuple(type_c<long>, type_c<float>));
//      static_assert(make_basic_tuple(type_c<int>, type_c<double>, type_c<long>, type_c<float>) == result, "");
//   }
//
//   TEST_CASE("hana concat tuple_t") {
//      constexpr auto result = concat(tuple_t<int, double>,
//                                     tuple_t<long, float>);
//      static_assert(tuple_t<int, double, long, float> == result, "");
//   }
//
//   template <typename ... Ts>
//   struct Concats {
//      constexpr static auto list = fold_left(make_tuple(tuple_t<Ts>...), concat);
//   };
//
//   TEST_CASE("hana concat tuple_t ...") {
//      constexpr auto result = concat(tuple_t<int, double>,
//                                     tuple_t<long, float>);
//      static_assert(tuple_t<int, double, long> == Concats<int, double, long>::list, "");
//   }
//
//   TEST_CASE("hana unique") {
//      auto result = unique(tuple_t<int, double, double, float, int, float>);
//      static_assert(tuple_t<int, double, float, int, float> == result);
//   }
//
//
//   TEST_CASE("make unique") {
//      constexpr auto result = tuple_t<float, int, double, double, int, float>;
//      static_assert(tuple_t<float, int, double> == GRAPH_DSL_NS::unique(result));
//   }
//
//   TEST_CASE("hana group") {
//      auto result = partition(tuple_t<int, double, double, int>, [](auto x) {
//         return x == type_c<int>;
//      });
//      static_assert(make_pair(tuple_t<int, int>, tuple_t<double, double>) == result);
//   }
//
//}
