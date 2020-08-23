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
   device_info const* device_info_{};
   size_t size_{};

   auto get_device_info(uint8_t device_id) const -> struct device_info const* {
      for(size_t i=0; i < size_; i++) {
         if(device_info_[i].device_id == device_id) {
            return device_info_ + i;
         }
      }
      return nullptr;
   }

   auto cleanup() {
       device_info_ = nullptr;
       size_ = 0;
   }
};

inline constexpr auto operator==(root_state const& lhs, root_state const& rhs) noexcept -> bool {
   if(lhs.size_ != rhs.size_) return false;
   for(auto i=0; i<lhs.size_; i++) {
      if(lhs.device_info_[i] != rhs.device_info_[i]) return false;
   }
   return true;
}

inline constexpr auto operator!=(root_state const& lhs, root_state const& rhs) noexcept -> bool {
   return !operator==(lhs, rhs);
}

GRAPH_DSL_NS_END

#endif //GRAPH_ROOT_STATE_H
