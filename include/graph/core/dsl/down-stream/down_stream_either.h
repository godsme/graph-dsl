//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_DOWN_STREAM_EITHER_H
#define GRAPH_DOWN_STREAM_EITHER_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/GraphContext.h>
#include <graph/util/result_t.h>
#include <graph/core/NodeIndex.h>
#include <graph/core/dsl/down-stream/down_stream_trait_decl.h>
#include <holo/holo.h>
#include <maco/basic.h>
#include <vector>

GRAPH_DSL_NS_BEGIN

template<typename COND, typename NODE_LIKE_1, typename NODE_LIKE_2>
struct down_stream_either {
   using decorated_node_1 = typename down_stream_trait<NODE_LIKE_1>::type;
   using decorated_node_2 = typename down_stream_trait<NODE_LIKE_2>::type;

   constexpr static auto node_list = \
      holo::concat( down_stream_trait<NODE_LIKE_1>::type::node_list
                  , down_stream_trait<NODE_LIKE_2>::type::node_list);

   template<typename TUPLE>
   struct instance_type {
   private:
      using node_1 = typename decorated_node_1::template instance_type<TUPLE>;
      using node_2 = typename decorated_node_2::template instance_type<TUPLE>;

      template<size_t FROM, size_t TO, typename NODE>
      auto move_to(GraphContext& context) -> Status {
         if(node_.index() == FROM) {
            std::get<FROM>(node_).Release(context);
         }
         if(node_.index() != TO) {
            node_ = NODE{};
            GRAPH_EXPECT_SUCC(std::get<TO>(node_).Build(context));
         }
         return Status::Ok;
      }

      template<size_t N>
      inline auto Cleanup(GraphContext& context) {
         std::get<N>(node_).Release(context);
         node_ = std::monostate{};
      }
   public:
      auto Build(GraphContext& context) -> Status {
         return COND{}(context).with_value([&](auto satisfied) {
            if(satisfied) { return move_to<2, 1, node_1>(context); }
            else          { return move_to<1, 2, node_2>(context); }
         });
      }

      auto Release(GraphContext& context) {
         switch(node_.index()) {
            case 1: { Cleanup<1>(context); break; }
            case 2: { Cleanup<2>(context); break; }
            default: break;
         }
      }

      auto CollectActorHandle(GraphContext& context, ActorHandleSet & actor_handles) -> Status {
         switch(node_.index()) {
            case 1: return std::get<1>(node_).CollectActorHandle(context, actor_handles);
            case 2: return std::get<2>(node_).CollectActorHandle(context, actor_handles);
            default: return Status::Failed;
         }
      }

      auto Enabled() const -> bool { return node_.index() != 0; }

   private:
      std::variant<std::monostate, node_1, node_2> node_;
   };
};

template<typename COND, typename NODE_LIKE_1, typename NODE_LIKE_2>
struct down_stream_trait<down_stream_either<COND, NODE_LIKE_1, NODE_LIKE_2>, void> {
   using type = down_stream_either<COND, NODE_LIKE_1, NODE_LIKE_2>;
};

GRAPH_DSL_NS_END

#define __g_EITHER(...) __MACO_template_type(GRAPH_DSL_NS::down_stream_either<__VA_ARGS__>)

#endif //GRAPH_DOWN_STREAM_EITHER_H
