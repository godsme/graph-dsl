//
// Created by Darwin Yuan on 2020/8/21.
//

#ifndef GRAPH_MULTI_DEVICES_H
#define GRAPH_MULTI_DEVICES_H

#include <graph/graph_ns.h>
#include <graph/core/dsl/target_state_selector.h>
#include <graph/core/dsl/state_transition.h>

GRAPH_DSL_NS_BEGIN

template<typename STATE_SELECTOR, typename STATE_TRANSITION>
struct MultiDevice {
   template<typename ENV>
   auto GetTransitions(const ENV& env) -> StatePath {
       m_targetState = STATE_SELECTOR::find(env);
       if(m_targetState.size == 0) {
           return {};
       }
       if(m_state.size == 0) {
         return { .state = &m_targetState, .size = 1 };
       }

       if(m_state == m_targetState) return {};
       return STATE_TRANSITION::find(m_state, m_targetState);
   }

   auto UpdateState(RootState const& state) {
       m_state = state;
   }

   auto CleanUp() {
       m_state.CleanUp();
       m_targetState.CleanUp();
   }

private:
   RootState m_state{};
   RootState m_targetState{};
};

GRAPH_DSL_NS_END

#endif //GRAPH_MULTI_DEVICES_H
