//
// Created by Darwin Yuan on 2020/8/20.
//

#include <catch.hpp>
#include <graph/core/dsl/state_transition.h>
#include <graph/core/transition_shortcut_search.h>
#include <iostream>

struct device_0 {
   constexpr static uint8_t Device_Id = 0;
};

struct device_1 {
   constexpr static uint8_t Device_Id = 1;
};

struct device_2 {
   constexpr static uint8_t Device_Id = 2;
};

struct device_3 {
   constexpr static uint8_t Device_Id = 3;
};

using namespace boost::hana;

namespace {
using trans =
__g_STATE_TRANSITIONS(
   ((device_0, __g_PREVIEW(device_1)) -> __g_PREVIEW(device_2), device_1),
   ((device_2, __g_PREVIEW(device_1)) -> device_0, __g_PREVIEW(device_1)),
   ((__g_PREVIEW(device_0), device_1) -> device_2, __g_PREVIEW(device_1)));

   template<typename T> struct S;
   TEST_CASE("state transitions") {
      constexpr auto result = trans::All_Direct_Transitions;
      constexpr auto expected = make_tuple(
       make_pair(
         type_c<graph_dsl::device_state<device_0, __g_PREVIEW(device_1)>>,
         type_c<graph_dsl::device_state<__g_PREVIEW(device_2), device_1>>),
       make_pair(
         type_c<graph_dsl::device_state<device_2, __g_PREVIEW(device_1)>>,
         type_c<graph_dsl::device_state<device_0, __g_PREVIEW(device_1)>>),
       make_pair(
          type_c<graph_dsl::device_state<__g_PREVIEW(device_0), device_1>>,
          type_c<graph_dsl::device_state<device_2, __g_PREVIEW(device_1)>>));

      static_assert(expected == result);
   }

   TEST_CASE("find a direct transitions") {
      constexpr auto from = type_c<graph_dsl::device_state<device_0, __g_PREVIEW(device_1)>>;
      constexpr auto to   = type_c<graph_dsl::device_state<__g_PREVIEW(device_2), device_1>>;

      constexpr auto result = GRAPH_DSL_NS::state_transition_algo::find_shortcut(from, to, trans::All_Direct_Transitions);
      static_assert(result == make_tuple(from, to));
   }

   TEST_CASE("find a indirect transition") {
      constexpr auto from = type_c<graph_dsl::device_state<__g_PREVIEW(device_0), device_1>>;
      constexpr auto to   = type_c<graph_dsl::device_state<device_0, __g_PREVIEW(device_1)>>;

      constexpr auto result = GRAPH_DSL_NS::state_transition_algo::find_shortcut(from, to, trans::All_Direct_Transitions);

      constexpr auto middle = type_c<graph_dsl::device_state<device_2, __g_PREVIEW(device_1)>>;
      static_assert(result == make_tuple(from, middle, to));
   }

using trans_2 =
   __g_STATE_TRANSITIONS(
      ((device_0, __g_PREVIEW(device_1)) -> __g_PREVIEW(device_2), device_1),
      ((__g_PREVIEW(device_2), device_1) -> device_0, __g_PREVIEW(device_1)),
      ((device_2, __g_PREVIEW(device_1)) -> __g_PREVIEW(device_0), device_1),
      ((__g_PREVIEW(device_0), device_1) -> device_2, __g_PREVIEW(device_1)),
      ((__g_PREVIEW(device_0), device_1) -> device_1, __g_PREVIEW(device_2)),
      ((device_1, __g_PREVIEW(device_2)) -> __g_PREVIEW(device_1), device_3),
      ((__g_PREVIEW(device_1), device_3) -> __g_PREVIEW(device_2), device_3),
      ((__g_PREVIEW(device_2), device_3) -> device_2, __g_PREVIEW(device_3))
      );

   TEST_CASE("find a long indirect transition") {
      auto from = type_c<graph_dsl::device_state<device_2, __g_PREVIEW(device_1)>>;
      auto to   = type_c<graph_dsl::device_state<device_2, __g_PREVIEW(device_3)>>;

      constexpr auto result = GRAPH_DSL_NS::state_transition_algo::find_shortcut(from, to, trans_2::All_Direct_Transitions);
      static_assert(size(result) == size_c<6>);

      constexpr auto m1 = type_c<graph_dsl::device_state<__g_PREVIEW(device_0), device_1>>;
      constexpr auto m2 = type_c<graph_dsl::device_state<device_1, __g_PREVIEW(device_2)>>;
      constexpr auto m3 = type_c<graph_dsl::device_state<__g_PREVIEW(device_1), device_3>>;
      constexpr auto m4 = type_c<graph_dsl::device_state<__g_PREVIEW(device_2), device_3>>;

      static_assert(result == make_tuple(from, m1, m2, m3, m4, to));
   }
}