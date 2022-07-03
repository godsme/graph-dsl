//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_DOWN_STREAM_EITHER_H
#define GRAPH_DOWN_STREAM_EITHER_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/util/result_t.h>
#include <graph/core/dsl/down-stream/down_stream_trait_decl.h>
#include <holo/holo.h>
#include <maco/basic.h>

GRAPH_DSL_NS_BEGIN

struct GraphContext;

template<typename COND, typename NODE_LIKE_1, typename NODE_LIKE_2>
struct DownStreamEither {
   using DecoratedNode1 = typename DownStreamTrait<NODE_LIKE_1>::Type;
   using DecoratedNode2 = typename DownStreamTrait<NODE_LIKE_2>::Type;

   constexpr static auto NODE_LIST = \
      holo::concat( DownStreamTrait<NODE_LIKE_1>::Type::NODE_LIST
                  , DownStreamTrait<NODE_LIKE_2>::Type::NODE_LIST);

   template<typename TUPLE>
   struct InstanceType {
   private:
      template<size_t FROM, size_t TO>
      auto Transfer(GraphContext& context) -> Status {
         if(m_node.index() == FROM) {
            std::get<FROM>(m_node).Release(context);
         }
         if(m_node.index() != TO) {
            m_node.template emplace<TO>();
            GRAPH_EXPECT_SUCC(std::get<TO>(m_node).Build(context));
         }
         return Status::OK;
      }

      template<size_t N>
      inline auto CleanUp(GraphContext& context) {
         std::get<N>(m_node).Release(context);
         m_node = std::monostate{};
      }

   public:
      auto Build(GraphContext& context) -> Status {
         return COND::Satisfied(context).with_value([&](auto satisfied) {
            if(satisfied) { return Transfer<2, 1>(context); }
            else          { return Transfer<1, 2>(context); }
         });
      }

      auto Release(GraphContext& context) {
         switch(m_node.index()) {
            case 1: { CleanUp<1>(context); break; }
            case 2: { CleanUp<2>(context); break; }
            default: break;
         }
      }

      auto CollectActorHandle(GraphContext& context, ActorHandleSet & actors) -> Status {
         switch(m_node.index()) {
            case 1: return std::get<1>(m_node).CollectActorHandle(context, actors);
            case 2: return std::get<2>(m_node).CollectActorHandle(context, actors);
            default: return Status::FAILED;
         }
      }

      auto Enabled() const -> bool { return m_node.index() != 0; }

   private:
       using Node1 = typename DecoratedNode1::template InstanceType<TUPLE>;
       using Node2 = typename DecoratedNode2::template InstanceType<TUPLE>;
      std::variant<std::monostate, Node1, Node2> m_node;
   };
};

template<typename COND, typename NODE_LIKE_1, typename NODE_LIKE_2>
struct DownStreamTrait<DownStreamEither<COND, NODE_LIKE_1, NODE_LIKE_2>, void> {
   using Type = DownStreamEither<COND, NODE_LIKE_1, NODE_LIKE_2>;
};

GRAPH_DSL_NS_END

#define __g_EITHER(...) __MACO_template_type(GRAPH_DSL_NS::DownStreamEither<__VA_ARGS__>)

#endif //GRAPH_DOWN_STREAM_EITHER_H
