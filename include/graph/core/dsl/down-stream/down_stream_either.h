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
#include <graph/core/dsl/down-stream/down_stream_trait_decl.h>
#include <holo/holo.h>
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
      auto move_to(graph_context& context) -> status_t {
         if(node_.index() == FROM) {
            std::get<FROM>(node_).release(context);
         }
         if(node_.index() != TO) {
            node_ = NODE{};
            GRAPH_EXPECT_SUCC(std::get<TO>(node_).build(context));
         }
         return status_t::Ok;
      }

      template<size_t N>
      inline auto cleanup(graph_context& context) {
         std::get<N>(node_).release(context);
         node_ = std::monostate{};
      }
   public:
      auto build(graph_context& context) -> status_t {
         return COND{}(context).with_value([&](auto satisfied) {
            if(satisfied) { return move_to<2, 1, node_1>(context); }
            else          { return move_to<1, 2, node_2>(context); }
         });
      }

      auto release(graph_context& context) {
         switch(node_.index()) {
            case 1: { cleanup<1>(context); break; }
            case 2: { cleanup<2>(context); break; }
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

      auto enabled() const -> bool { return node_.index() != 0; }

   private:
      std::variant<std::monostate, node_1, node_2> node_;
   };
};

template<typename COND, typename NODE_LIKE_1, typename NODE_LIKE_2>
struct down_stream_trait<down_stream_either<COND, NODE_LIKE_1, NODE_LIKE_2>, void> {
   using type = down_stream_either<COND, NODE_LIKE_1, NODE_LIKE_2>;
};

GRAPH_DSL_NS_END

#define __g_EITHER(...) std::decay_t<decltype(std::declval<GRAPH_DSL_NS::down_stream_either<__VA_ARGS__>>())>

#endif //GRAPH_DOWN_STREAM_EITHER_H
