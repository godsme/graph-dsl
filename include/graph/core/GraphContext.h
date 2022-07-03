//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_GRAPH_CONTEXT_H
#define GRAPH_GRAPH_CONTEXT_H

#include <graph/graph_ns.h>
#include <graph/core/RootState.h>

GRAPH_DSL_NS_BEGIN

struct GraphContext {
    GraphContext() = default;

   template<typename NODES, int Index>
   inline auto GetNode() -> decltype(auto) {
      return (std::get<Index>(*reinterpret_cast<NODES*>(m_subgraphNodes)));
   }

   template<typename NODES, int Index>
   inline auto GetRootNode() -> decltype(auto) {
      return (std::get<Index>(*reinterpret_cast<NODES*>(m_roots)));
   }

   template<typename ROOT_NODES>
   auto UpdateRootNodes(ROOT_NODES& nodes) {
       m_roots = reinterpret_cast<void*>(&nodes);
   }

   template<typename NODES>
   auto SwitchSubgraphContext(NODES& nodes) {
       m_subgraphNodes = reinterpret_cast<void*>(&nodes);
   }

   auto GetRootState() const -> decltype(auto) {
      return (m_rootState);
   }

   auto UpdateRootState(const RootState& state) {
       m_rootState = state;
   }

private:
   void*                    m_roots{};
   void*                    m_subgraphNodes{};
   RootState                m_rootState{};
};

GRAPH_DSL_NS_END

#endif //GRAPH_GRAPH_CONTEXT_H
