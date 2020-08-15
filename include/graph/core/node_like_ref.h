//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_NODE_LIKE_REF_H
#define GRAPH_NODE_LIKE_REF_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/core/node_index.h>
#include <graph/core/node_like_trait_decl.h>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

struct node_signature {};

template <typename NODE>
struct node_like_ref {
   constexpr static auto node_list = hana::tuple_t<NODE>;

   template<typename TUPLE>
   struct instance_type : node_index<NODE, TUPLE> {
      auto build(graph_context& context) -> status_t {
         if(!enabled_) {
            node_index<NODE, TUPLE>::get_node(context).add_ref();
            enabled_ = true;
         }

         return status_t::Ok;
      }

      auto release(graph_context& context) {
         node_index<NODE, TUPLE>::get_node(context).release();
         enabled_ = false;
      }

      auto enabled() const -> bool {
         return enabled_;
      }

   private:
      bool enabled_{false};
   };
};

template<typename NODE>
struct node_like_trait<NODE, std::enable_if_t<std::is_base_of_v<node_signature, NODE>>> {
   using type = node_like_ref<NODE>;
};

GRAPH_DSL_NS_END

#endif //GRAPH_NODE_LIKE_REF_H
