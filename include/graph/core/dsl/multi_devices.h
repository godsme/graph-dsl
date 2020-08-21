//
// Created by Darwin Yuan on 2020/8/21.
//

#ifndef GRAPH_MULTI_DEVICES_H
#define GRAPH_MULTI_DEVICES_H

#include <graph/graph_ns.h>
#include <graph/core/dsl/target_state_selector.h>
#include <graph/core/dsl/state_transition.h>

GRAPH_DSL_NS_BEGIN

template<typename INIT_STATE, typename STATE_SELECTOR, typename STATE_TRANSITION>
struct multi_device {
   constexpr static root_state Init_State = INIT_STATE::Root_State;

   template<typename ENV>
   auto get_transitions(const ENV& env) -> state_path {
      root_state target = STATE_SELECTOR::find(env);
      if(target.size == 0) {
         return {};
      } else {
         state_path result = STATE_TRANSITION::find(state_, target);
         if(result.size > 1) {
            state_ = *result.get_last();
         }
         return result;
      }
   }

private:
   root_state state_{Init_State};
};

GRAPH_DSL_NS_END

#endif //GRAPH_MULTI_DEVICES_H
