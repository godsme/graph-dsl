//
// Created by Darwin Yuan on 2020/8/21.
//

#ifndef GRAPH_ROOT_STATE_H
#define GRAPH_ROOT_STATE_H

#include <graph/graph_ns.h>
#include <cstdint>
#include <tuple>
#include <graph/core/device_info.h>

GRAPH_DSL_NS_BEGIN

struct root_state {
   device_info const* device_info{};
   size_t size{};

   auto get_device_info(uint8_t device_id) const -> struct device_info const* {
      for(size_t i=0; i<size; i++) {
         if(device_info[i].device_id == device_id) {
            return device_info + i;
         }
      }
      return nullptr;
   }

   auto cleanup() {
      device_info = nullptr;
      size = 0;
   }
};

inline constexpr auto operator==(root_state const& lhs, root_state const& rhs) noexcept -> bool {
   if(lhs.size != rhs.size) return false;
   for(auto i=0; i<lhs.size; i++) {
      if(lhs.device_info[i] != rhs.device_info[i]) return false;
   }
   return true;
}

inline constexpr auto operator!=(root_state const& lhs, root_state const& rhs) noexcept -> bool {
   return !operator==(lhs, rhs);
}

GRAPH_DSL_NS_END

#endif //GRAPH_ROOT_STATE_H
