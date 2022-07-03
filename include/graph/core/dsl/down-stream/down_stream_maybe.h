//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_DOWN_STREAM_MAYBE_H
#define GRAPH_DOWN_STREAM_MAYBE_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/GraphContext.h>
#include <graph/util/result_t.h>
#include <graph/core/NodeIndex.h>
#include <graph/core/dsl/down-stream/down_stream_trait_decl.h>
#include <vector>

GRAPH_DSL_NS_BEGIN

//////////////////////////////////////////////////////////////////////////////
template<typename COND, typename NODE_LIKE>
struct down_stream_maybe {
   using decorated_node = typename down_stream_trait<NODE_LIKE>::type;
   constexpr static auto node_list = decorated_node::node_list;

   template<typename TUPLE>
   struct instance_type {
      auto Build(GraphContext& context) -> Status {
         return COND{}(context).with_value([&](auto satisfied) {
            if(satisfied) {
               return Build_(context);
            } else {
               Release(context);
               return Status::Ok;
            }
         });
      }

      auto Release(GraphContext& context) {
         if(satisfied_) {
            node_.Release(context);
            satisfied_ = false;
         }
      }

      auto CollectActorHandle(GraphContext& context, ActorHandleSet& actor_handles) -> Status {
         GRAPH_EXPECT_TRUE(Enabled());
         return node_.CollectActorHandle(context, actor_handles);
      }

      auto Enabled() const -> bool { return satisfied_; }

   private:
      auto Build_(GraphContext& context) -> Status {
         if(!satisfied_) {
            GRAPH_EXPECT_SUCC(node_.Build(context));
            satisfied_ = true;
         }
         return Status::Ok;
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

#define __g_MAYBE(...) __MACO_template_type(GRAPH_DSL_NS::down_stream_maybe<__VA_ARGS__>)

#endif //GRAPH_DOWN_STREAM_MAYBE_H
