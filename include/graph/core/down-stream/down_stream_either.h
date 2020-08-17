//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_DOWN_STREAM_EITHER_H
#define GRAPH_DOWN_STREAM_EITHER_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/util/result_t.h>
#include <graph/core/node_index.h>
#include <graph/core/down-stream/down_stream_trait_decl.h>
#include <boost/hana/fwd/tuple.hpp>
#include <boost/hana/concat.hpp>
#include <vector>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template<typename COND, typename NODE_LIKE_1, typename NODE_LIKE_2>
struct down_stream_either {
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
         return COND{}(context).with_value([&](auto satisfied) {
            if(satisfied) {
               if(node_.index() == 2) {
                  std::get<2>(node_).release(context);
               }
               if(node_.index() != 1) {
                  node_ = node_1{};
                  GRAPH_EXPECT_SUCC(std::get<1>(node_).build(context));
               }
            } else {
               if(node_.index() == 1) {
                  std::get<1>(node_).release(context);
               }
               if(node_.index() != 2) {
                  node_ = node_2{};
                  GRAPH_EXPECT_SUCC(std::get<2>(node_).build(context));
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

      auto collect_actor_handle(graph_context& context, actor_handle_set& actor_handles) -> status_t {
         switch(node_.index()) {
            case 1: return std::get<1>(node_).collect_actor_handle(context, actor_handles);
            case 2: return std::get<2>(node_).collect_actor_handle(context, actor_handles);
            default: return status_t::Failed;
         }
      }

      auto enabled() const -> bool {
         return node_.index() != 0;
      }

   private:
      std::variant<std::monostate, node_1, node_2> node_;
   };
};

template<typename COND, typename NODE_LIKE_1, typename NODE_LIKE_2>
struct node_like_trait<down_stream_either<COND, NODE_LIKE_1, NODE_LIKE_2>, void> {
   using type = down_stream_either<COND, NODE_LIKE_1, NODE_LIKE_2>;
};

GRAPH_DSL_NS_END

#define __g_EITHER(...) GRAPH_DSL_NS::down_stream_either<__VA_ARGS__>

#endif //GRAPH_DOWN_STREAM_EITHER_H
