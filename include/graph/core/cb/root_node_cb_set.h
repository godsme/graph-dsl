//
// Created by Darwin Yuan on 2020/8/17.
//

#ifndef GRAPH_ROOT_NODE_CB_SET_H
#define GRAPH_ROOT_NODE_CB_SET_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <cstdint>
#include <cstddef>
#include <graph/core/cb/root_node_cb.h>

GRAPH_DSL_NS_BEGIN

struct root_node_cb_set {
   virtual auto get_root_node(size_t index) -> root_node_cb* = 0;
   virtual ~root_node_cb_set() = default;
};

template<typename ... Ts>
struct root_nodes : root_node_cb_set {
   root_nodes() {
      push_to_container(std::make_index_sequence<sizeof...(Ts)>{});
   }

   auto get_root_node(size_t index) -> root_node_cb* {
      if(index >= sizeof...(Ts)) {
         return nullptr;
      }

      return containers_[index];
   }

   template <size_t I>
   auto get() -> decltype(auto) {
      static_assert(I <= sizeof...(Ts), "");
      return (std::get<I>(nodes_));
   }

private:
   template <size_t ... I>
   auto push_to_container(std::index_sequence<I...>) {
      ((containers_.push_back(static_cast<root_node_cb*>(&std::get<I>(nodes_)))), ...);
   }

private:
   std::vector<root_node_cb*> containers_;
   std::tuple<generic_root_node_cb<Ts>...> nodes_;
};

GRAPH_DSL_NS_END

#endif //GRAPH_ROOT_NODE_CB_SET_H
