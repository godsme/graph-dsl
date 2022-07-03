//
// Created by Darwin Yuan on 2020/8/21.
//

#ifndef GRAPH_ROOT_STATE_H
#define GRAPH_ROOT_STATE_H

#include <graph/graph_ns.h>
#include <cstdint>
#include <tuple>
#include <graph/core/DeviceInfo.h>

GRAPH_DSL_NS_BEGIN

struct RootState {
   DeviceInfo const* deviceInfo{};
   size_t size{};

   auto GetDeviceInfo(uint8_t deviceId) const -> struct DeviceInfo const* {
      for(size_t i=0; i < size; i++) {
         if(deviceInfo[i].deviceId == deviceId) {
            return deviceInfo + i;
         }
      }
      return nullptr;
   }

   auto CleanUp() {
       deviceInfo = nullptr;
       size = 0;
   }
};

inline constexpr auto operator==(RootState const& lhs, RootState const& rhs) noexcept -> bool {
   if(lhs.size != rhs.size) return false;
   for(auto i=0; i<lhs.size; i++) {
      if(lhs.deviceInfo[i] != rhs.deviceInfo[i]) return false;
   }
   return true;
}

inline constexpr auto operator!=(RootState const& lhs, RootState const& rhs) noexcept -> bool {
   return !operator==(lhs, rhs);
}

GRAPH_DSL_NS_END

#endif //GRAPH_ROOT_STATE_H
