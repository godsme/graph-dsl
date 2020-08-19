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

namespace {

   TEST_CASE("devices") {
      static_assert(__g_STATE(device_0, __g_PREVIEW(device_1))::Num_Of_Devices == 2);

      static_assert(__g_STATE(device_0, __g_PREVIEW(device_1))::Devices[0].device_id == 0);
      static_assert(!__g_STATE(device_0, __g_PREVIEW(device_1))::Devices[0].is_preview);
      
      static_assert(__g_STATE(device_0, __g_PREVIEW(device_1))::Devices[1].device_id == 1);
      static_assert(__g_STATE(device_0, __g_PREVIEW(device_1))::Devices[1].is_preview);
   }

}