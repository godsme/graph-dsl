//
// Created by Darwin Yuan on 2020/8/21.
//

#ifndef GRAPH_DEVICE_INFO_H
#define GRAPH_DEVICE_INFO_H

#include <graph/graph_ns.h>
#include <cstdint>
#include <tuple>

GRAPH_DSL_NS_BEGIN

struct DeviceInfo {
   const uint8_t deviceId;
   const bool isPreview;

   inline constexpr auto Tie() const noexcept {
      return std::tie(deviceId, isPreview);
   }
};

inline constexpr auto operator==(DeviceInfo const& lhs, DeviceInfo const& rhs) noexcept -> bool {
   return lhs.Tie() == rhs.Tie();
}

inline constexpr auto operator!=(DeviceInfo const& lhs, DeviceInfo const& rhs) noexcept -> bool {
   return !operator==(lhs, rhs);
}

inline constexpr auto operator<(DeviceInfo const& lhs, DeviceInfo const& rhs) noexcept -> bool {
   return lhs.Tie() < rhs.Tie();
}

inline constexpr auto operator<=(DeviceInfo const& lhs, DeviceInfo const& rhs) noexcept -> bool {
   return lhs.Tie() <= rhs.Tie();
}

inline constexpr auto operator>(DeviceInfo const& lhs, DeviceInfo const& rhs) noexcept -> bool {
   return lhs.Tie() > rhs.Tie();
}

inline constexpr auto operator>=(DeviceInfo const& lhs, DeviceInfo const& rhs) noexcept -> bool {
   return lhs.Tie() >= rhs.Tie();
}

GRAPH_DSL_NS_END

#endif //GRAPH_DEVICE_INFO_H
