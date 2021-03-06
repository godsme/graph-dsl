//
// Created by Darwin Yuan on 2020/8/21.
//

#ifndef GRAPH_MULTI_DEVICES_H
#define GRAPH_MULTI_DEVICES_H

#include <graph/graph_ns.h>
#include <graph/core/dsl/target_state_selector.h>
#include <graph/core/dsl/state_transition.h>
#include <spdlog/spdlog.h>

GRAPH_DSL_NS_BEGIN

template<typename STATE_SELECTOR, typename STATE_TRANSITION>
struct multi_device {
   template<typename ENV>
   auto get_transitions(const ENV& env) -> state_path {
      target_state_ = STATE_SELECTOR::find(env);
      if(target_state_.size_ == 0) {
         spdlog::error("no state selected");
         return {};
      }
      if(state_.size_ == 0) {
         return { .state = &target_state_, .size = 1 };
      }

      spdlog::critical("{} ({}-{}) -> {} ({}-{}) ",
                    state_.size_,
                           state_.device_info_[0].is_preview ?
                           state_.device_info_[0].device_id :
                           state_.device_info_[1].device_id,
                           state_.device_info_[0].is_preview ?
                           state_.device_info_[1].device_id :
                           state_.device_info_[0].device_id,
                    target_state_.size_,
                    target_state_.device_info_[0].is_preview ?
                    target_state_.device_info_[0].device_id :
                    target_state_.device_info_[1].device_id,
                    target_state_.device_info_[0].is_preview ?
                    target_state_.device_info_[1].device_id :
                    target_state_.device_info_[0].device_id);

      if(state_ == target_state_) return {};
      return STATE_TRANSITION::find(state_, target_state_);
   }

   auto update_state(root_state const& state) {
      state_ = state;
   }

   auto cleanup() {
      state_.cleanup();
      target_state_.cleanup();
   }

private:
   root_state state_{};
   root_state target_state_{};
};

GRAPH_DSL_NS_END

#endif //GRAPH_MULTI_DEVICES_H
