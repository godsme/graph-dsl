//
// Created by Darwin Yuan on 2020/8/20.
//

#include <catch.hpp>
#include <graph/core/dsl/target_state_selector.h>

struct device_0 {
   constexpr static uint8_t Device_Id = 0;
};

struct device_1 {
   constexpr static uint8_t Device_Id = 1;
};

struct device_2 {
   constexpr static uint8_t Device_Id = 2;
};

namespace {

   TEST_CASE("devices") {
      static_assert(__g_STATE(device_0, __g_PREVIEW(device_1))::Num_Of_Devices == 2);

      static_assert(__g_STATE(device_0, __g_PREVIEW(device_1))::sorted_devices::Devices[0].device_id == 0);
      static_assert(!__g_STATE(device_0, __g_PREVIEW(device_1))::sorted_devices::Devices[0].is_preview);

      static_assert(__g_STATE(device_0, __g_PREVIEW(device_1))::sorted_devices::Devices[1].device_id == 1);
      static_assert(__g_STATE(device_0, __g_PREVIEW(device_1))::sorted_devices::Devices[1].is_preview);
   }

   struct dict {
      auto get_scene() const -> uint8_t {
         return scene;
      }
      auto get_condition_1() const -> double {
         return condition_1;
      }
      auto get_condition_2() const -> unsigned int {
         return condition_2;
      }
      auto get_condition_3() const -> unsigned int {
         return condition_3;
      }

      uint8_t scene{1};
      double condition_1{1.5};
      unsigned int condition_2{4};
      unsigned int condition_3{6};
   };

   using selector =
      __g_STATE_SELECTOR(
         __g_WHEN(__g_SCENE_MODE(1), __g_COND_1( [1.0, 2.0} ))
      -> __g_STATE(device_0, __g_PREVIEW(device_1)),

         __g_WHEN(__g_SCENE_MODE(1), __g_COND_1( [1.0, 2.0} ), __g_COND_2(1, 5))
      -> __g_STATE(__g_PREVIEW(device_0), device_1),

         __g_WHEN(__g_SCENE_MODE(1), __g_COND_1( [1.5, 2.0} ), __g_COND_2(1, 5), __g_COND_3(3, 5))
      -> __g_STATE(device_2, __g_PREVIEW(device_1)));

   TEST_CASE("selector 1") {
      auto [device_info, size] = selector::sorted_entries::find(dict{});
      REQUIRE(device_info != nullptr);
      REQUIRE(size == 2);
      REQUIRE(device_info[0].device_id == 0);
      REQUIRE(device_info[1].device_id == 1);
      REQUIRE(device_info[0].is_preview);
      REQUIRE_FALSE(device_info[1].is_preview);
   }

   TEST_CASE("selector 2") {
      dict dict2;
      dict2.condition_3 = 4;
      auto [device_info, size] = selector::sorted_entries::find(dict2);
      REQUIRE(device_info != nullptr);
      REQUIRE(size == 2);
      REQUIRE(device_info[0].device_id == 1);
      REQUIRE(device_info[1].device_id == 2);
      REQUIRE(device_info[0].is_preview);
      REQUIRE_FALSE(device_info[1].is_preview);
   }

   TEST_CASE("selector 3") {
      dict dict2;
      dict2.condition_2 = 6;
      dict2.condition_3 = 4;
      auto [device_info, size] = selector::sorted_entries::find(dict2);
      REQUIRE(device_info != nullptr);
      REQUIRE(size == 2);
      REQUIRE(device_info[0].device_id == 0);
      REQUIRE(device_info[1].device_id == 1);
      REQUIRE_FALSE(device_info[0].is_preview);
      REQUIRE(device_info[1].is_preview);
   }

   template<typename = void, typename ... TRANS>
   struct state_transitions {
   };

//#define __StAtE_transform_2(empty, state1, arrow, state2, ...) , auto ( __g_STATE state1) arrow __g_STATE state2
//#define __StAtE_transform(x) __StAtE_transform_2 x
//#define __StAtE_each_transition(n, x) __StAtE_transform(x)
//#define __state(...) , (__VA_ARGS__) ,
//#define __state_transition_table(...) \
//state_transitions<void __CUB_overload(__CUB_repeat_call_, __VA_ARGS__) (__StAtE_each_transition, 0, __VA_ARGS__)>

//using transitions =
//__state_transition_table(
//   (__state(device_0, __g_PREVIEW(device_1)) -> __state(__g_PREVIEW(device_2), device_1)),
//   (__state(__g_PREVIEW(device_2), device_1) -> __state(device_0, __g_PREVIEW(device_1)))
//   );

   template<typename ... Ts> struct transition_trait;
   template<typename FROM, typename TO1, typename ... TOs>
   struct transition_trait<auto (FROM) -> TO1, TOs...> {
      using from_state = FROM;
      using to_state = GRAPH_DSL_NS::device_state<TO1, TOs...>;
   };

#define __graph_StAtE_transform_2(...)   auto ( GRAPH_DSL_NS::device_state<__VA_ARGS__> )
#define __graph_StAtE_transform(...) transition_trait< __graph_StAtE_transform_2 __VA_ARGS__ >
#define __graph_StAtE_each_transition(n, x) , __graph_StAtE_transform x
#define __g_STATE_TRANSITIONS(...) \
state_transitions<void __CUB_overload(__CUB_repeat_call_, __VA_ARGS__) (__graph_StAtE_each_transition, 0, __VA_ARGS__)>

using trans =
__g_STATE_TRANSITIONS(
   ((device_0, __g_PREVIEW(device_1)) -> __g_PREVIEW(device_2), device_1),
   ((__g_PREVIEW(device_2), device_1) -> device_0, __g_PREVIEW(device_1))
   );

}