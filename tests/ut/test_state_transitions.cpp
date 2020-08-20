//
// Created by Darwin Yuan on 2020/8/20.
//

#include <catch.hpp>
#include <graph/core/dsl/state_transition.h>

struct device_0 {
   constexpr static uint8_t Device_Id = 0;
};

struct device_1 {
   constexpr static uint8_t Device_Id = 1;
};

struct device_2 {
   constexpr static uint8_t Device_Id = 2;
};

using namespace boost::hana;

namespace {
using trans =
__g_STATE_TRANSITIONS(
   ((device_0, __g_PREVIEW(device_1)) -> __g_PREVIEW(device_2), device_1),
   ((__g_PREVIEW(device_2), device_1) -> device_0, __g_PREVIEW(device_1)),
   ((__g_PREVIEW(device_0), device_1) -> __g_PREVIEW(device_2), device_1));

   template<typename T> struct S;
   TEST_CASE("state transitions") {
      constexpr auto result = trans::All_Direct_Transitions;
      constexpr auto expected = make_tuple(
       make_pair(
         type_c<graph_dsl::device_state<device_0, __g_PREVIEW(device_1)>>,
         type_c<graph_dsl::device_state<__g_PREVIEW(device_2), device_1>>),
       make_pair(
         type_c<graph_dsl::device_state<__g_PREVIEW(device_2), device_1>>,
         type_c<graph_dsl::device_state<device_0, __g_PREVIEW(device_1)>>),
       make_pair(
          type_c<graph_dsl::device_state<__g_PREVIEW(device_0), device_1>>,
          type_c<graph_dsl::device_state<__g_PREVIEW(device_2), device_1>>));

      static_assert(expected == result);
   }

   TEST_CASE("find a direct transitions") {
      constexpr auto transition = make_pair(
         type_c<graph_dsl::device_state<device_0, __g_PREVIEW(device_1)>>,
         type_c<graph_dsl::device_state<__g_PREVIEW(device_2), device_1>>);

      constexpr auto result = trans::find_shortcut(transition, trans::All_Direct_Transitions);
      static_assert(first(result) == true);
      static_assert(second(result) == transition);
      //S<decltype(result)> s;
   }

   TEST_CASE("find a indirect transition") {
      constexpr auto transition = make_pair(
         type_c<graph_dsl::device_state<device_2, __g_PREVIEW(device_1)>>,
         type_c<graph_dsl::device_state<__g_PREVIEW(device_2), device_1>>);

      constexpr auto result = trans::find_shortcut(transition, trans::All_Direct_Transitions);
      static_assert(first(result) == false);
   }
}