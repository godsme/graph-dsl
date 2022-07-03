//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_DOWN_STREAM_MAYBE_H
#define GRAPH_DOWN_STREAM_MAYBE_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/util/result_t.h>
#include <graph/core/dsl/down-stream/down_stream_trait_decl.h>

GRAPH_DSL_NS_BEGIN

struct GraphContext;

//////////////////////////////////////////////////////////////////////////////
template<typename COND, typename NODE_LIKE>
struct DownStreamMaybe {
   using DecoratedNode = typename DownStreamTrait<NODE_LIKE>::Type;
   constexpr static auto NODE_LIST = DecoratedNode::NODE_LIST;

   template<typename TUPLE>
   struct InstanceType {
      auto Build(GraphContext& context) -> Status {
         return COND::Satisfied(context).with_value([&](auto satisfied) {
            if(satisfied) {
               return DoBuild(context);
            } else {
               Release(context);
               return Status::OK;
            }
         });
      }

      auto Release(GraphContext& context) {
         if(m_node) {
            m_node.Release(context);
            m_node.reset();
         }
      }

      auto CollectActorHandle(GraphContext& context, ActorHandleSet& actor_handles) -> Status {
         GRAPH_EXPECT_TRUE(Enabled());
         return m_node.CollectActorHandle(context, actor_handles);
      }

      auto Enabled() const -> bool { return m_node; }

   private:
      auto DoBuild(GraphContext& context) -> Status {
         if(!m_node) {
            m_node = NodeType{};
            GRAPH_EXPECT_SUCC(m_node.Build(context));
         }
         return Status::OK;
      }

   private:
       using NodeType = typename DecoratedNode::template InstanceType<TUPLE>;
       std::optional<NodeType> m_node;
   };
};

template<typename COND, typename NODE_LIKE>
struct DownStreamTrait<DownStreamMaybe<COND, NODE_LIKE>, void> {
   using Type = DownStreamMaybe<COND, NODE_LIKE>;
};

GRAPH_DSL_NS_END

#define __g_MAYBE(...) __MACO_template_type(GRAPH_DSL_NS::DownStreamMaybe<__VA_ARGS__>)

#endif //GRAPH_DOWN_STREAM_MAYBE_H
