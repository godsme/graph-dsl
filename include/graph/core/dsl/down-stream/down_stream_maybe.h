//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_DOWN_STREAM_MAYBE_H
#define GRAPH_DOWN_STREAM_MAYBE_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/util/result_t.h>
#include <graph/core/node_index.h>
#include <graph/core/dsl/down-stream/down_stream_trait_decl.h>
#include <boost/hana/fwd/tuple.hpp>
#include <vector>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

//////////////////////////////////////////////////////////////////////////////
template<typename COND, typename NODE_LIKE>
struct down_stream_maybe {
   using decorated_node = typename down_stream_trait<NODE_LIKE>::type;
   constexpr static auto node_list = decorated_node::node_list;

   template<typename TUPLE>
   struct instance_type {
      auto build(graph_context& context) -> status_t {
         return COND{}(context).with_value([&](auto satisfied) {
            if(satisfied) {
               return build_(context);
            } else {
               release(context);
            }
            return status_t::Ok;
         });
      }

      auto release(graph_context& context) {
         if(satisfied_) {
            node_.release(context);
            satisfied_ = false;
         }
      }

      auto collect_actor_handle(graph_context& context, actor_handle_set& actor_handles) -> status_t {
         GRAPH_EXPECT_TRUE(enabled());
         return node_.collect_actor_handle(context, actor_handles);
      }

      auto enabled() const -> bool {
         return satisfied_;
      }

   private:
      auto build_(graph_context& context) -> status_t {
         if(!satisfied_) {
            GRAPH_EXPECT_SUCC(node_.build(context));
            satisfied_ = true;
         }
         return status_t::Ok;
      }

   private:
      typename decorated_node::template instance_type<TUPLE> node_;
      bool satisfied_{false};
   };
};

template<typename COND, typename NODE_LIKE>
struct down_stream_trait<down_stream_maybe<COND, NODE_LIKE>, void> {
   using type = down_stream_maybe<COND, NODE_LIKE>;
};

GRAPH_DSL_NS_END

#define __g_MAYBE(...) GRAPH_DSL_NS::down_stream_maybe<__VA_ARGS__>

#endif //GRAPH_DOWN_STREAM_MAYBE_H
