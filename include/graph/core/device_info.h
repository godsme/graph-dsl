//
// Created by Darwin Yuan on 2020/8/21.
//

#ifndef GRAPH_DEVICE_INFO_H
#define GRAPH_DEVICE_INFO_H

#include <graph/graph_ns.h>
#include <cstdint>
#include <tuple>

GRAPH_DSL_NS_BEGIN

struct device_info {
   const uint8_t device_id;
   const bool is_preview;

   inline constexpr auto tie() const noexcept {
      return std::tie(device_id, is_preview);
   }
};

inline constexpr auto operator==(device_info const& lhs, device_info const& rhs) noexcept -> bool {
   return lhs.tie() == rhs.tie();
}

inline constexpr auto operator!=(device_info const& lhs, device_info const& rhs) noexcept -> bool {
   return !operator==(lhs, rhs);
}

inline constexpr auto operator<(device_info const& lhs, device_info const& rhs) noexcept -> bool {
   return lhs.tie() < rhs.tie();
}

inline constexpr auto operator<=(device_info const& lhs, device_info const& rhs) noexcept -> bool {
   return lhs.tie() <= rhs.tie();
}

inline constexpr auto operator>(device_info const& lhs, device_info const& rhs) noexcept -> bool {
   return lhs.tie() > rhs.tie();
}

inline constexpr auto operator>=(device_info const& lhs, device_info const& rhs) noexcept -> bool {
   return lhs.tie() >= rhs.tie();
}

GRAPH_DSL_NS_END

#endif //GRAPH_DEVICE_INFO_H
