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
