//
// Created by Darwin Yuan on 2020/8/21.
//

#include <catch.hpp>
#include <graph/core/dsl/multi_devices.h>

namespace {
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

   struct environment {
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
   -> __g_STATE(__g_PREVIEW(device_2), device_1),

      __g_WHEN(__g_SCENE_MODE(1), __g_COND_1( [1.5, 2.0} ), __g_COND_2(1, 5), __g_COND_3(3, 5))
   -> __g_STATE(device_2, __g_PREVIEW(device_1)),

      __g_WHEN(__g_SCENE_MODE(1), __g_COND_1( [2.0, 3.0} ), __g_COND_2(1, 5), __g_COND_3(3, 5))
   -> __g_STATE(device_1, __g_PREVIEW(device_0)),

      __g_WHEN(__g_SCENE_MODE(1), __g_COND_1( [2.0, 3.0} ), __g_COND_2( [5, 10} ), __g_COND_3(3, 5))
   -> __g_STATE(device_1, __g_PREVIEW(device_2)),

      __g_WHEN(__g_SCENE_MODE(1), __g_COND_1( [2.0, 3.0} ), __g_COND_2( [5, 10} ), __g_COND_3([5, 10}))
   -> __g_STATE(device_3, __g_PREVIEW(device_1)),

      __g_WHEN(__g_SCENE_MODE(1), __g_COND_1( [3.0, 4.0} ), __g_COND_2( [5, 10} ), __g_COND_3([5, 10}))
   -> __g_STATE(device_3, __g_PREVIEW(device_2)),

      __g_WHEN(__g_SCENE_MODE(1), __g_COND_1( [3.0, 4.0} ), __g_COND_2( [10, 15} ), __g_COND_3([5, 10}))
   -> __g_STATE(device_2, __g_PREVIEW(device_3)));

   using transitions =
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

   using init_state = __g_STATE(device_0, __g_PREVIEW(device_1));

   GRAPH_DSL_NS::multi_device<init_state, selector, transitions> multi_device_instance;

   TEST_CASE("select a direct transition") {
      environment env{};
      env.condition_3 = 15;
      auto result = multi_device_instance.get_transitions(env);
      REQUIRE(result.size == 2);
      REQUIRE(result.state[1] == __g_STATE(__g_PREVIEW(device_2), device_1)::Root_State);

      WHEN("condition changed") {
         env.condition_1 = 3.0;
         env.condition_2 = 12;
         env.condition_3 = 8;

         result = multi_device_instance.get_transitions(env);
         REQUIRE(result.size == 4);
         REQUIRE(result.state[0] == __g_STATE(__g_PREVIEW(device_2), device_1)::Root_State);
         REQUIRE(result.state[1] == __g_STATE(__g_PREVIEW(device_1), device_3)::Root_State);
         REQUIRE(result.state[2] == __g_STATE(__g_PREVIEW(device_2), device_3)::Root_State);
         REQUIRE(result.state[3] == __g_STATE(device_2, __g_PREVIEW(device_3))::Root_State);
      }
   }
}
