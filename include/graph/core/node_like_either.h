//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_NODE_LIKE_EITHER_H
#define GRAPH_NODE_LIKE_EITHER_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/util/result_t.h>
#include <graph/core/node_index.h>
#include <graph/core/node_like_trait.h>
#include <boost/hana/fwd/tuple.hpp>
#include <vector>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template<typename COND, typename NODE_LIKE_1, typename NODE_LIKE_2>
struct node_like_either {
   using decorated_node_1 = typename node_like_trait<NODE_LIKE_1>::type;
   using decorated_node_2 = typename node_like_trait<NODE_LIKE_2>::type;

   constexpr static auto node_list = \
      hana::concat
      ( node_like_trait<NODE_LIKE_1>::type::node_list
         , node_like_trait<NODE_LIKE_2>::type::node_list);

   template<typename TUPLE>
   struct instance_type {
   private:
      using node_1 = typename decorated_node_1::template instance_type<TUPLE>;
      using node_2 = typename decorated_node_2::template instance_type<TUPLE>;

   public:
      auto build(graph_context& context) -> status_t {
         return COND(context).with_value([&](auto satisfied) {
            if(satisfied) {
               if(node_.index() == 2) {
                  std::get<2>(node_).release(context);
               }
               if(node_.index() != 1) {
                  node_ = node_1{};
                  GRAPH_EXPECT_SUCC(node_.build(context));
               }
            } else {
               if(node_.index() == 1) {
                  std::get<1>(node_).release(context);
               }
               if(node_.index() != 2) {
                  node_ = node_2{};
                  GRAPH_EXPECT_SUCC(node_.build(context));
               }
            }
            return status_t::Ok;
         });
      }

      auto release(graph_context& context) {
         switch(node_.index()) {
            case 1: std::get<1>(node_).release(context); break;
            case 2: std::get<2>(node_).release(context); break;
            default: break;
         }
      }
   private:
      std::variant<std::monostate, node_1, node_2> node_;
   };
};

template<typename COND, typename NODE_LIKE_1, typename NODE_LIKE_2>
struct node_like_trait<node_like_either<COND, NODE_LIKE_1, NODE_LIKE_2>, void> {
   using type = node_like_either<COND, NODE_LIKE_1, NODE_LIKE_2>;
};

GRAPH_DSL_NS_END

#endif //GRAPH_NODE_LIKE_EITHER_H
