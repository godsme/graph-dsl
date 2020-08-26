//
// Created by Darwin Yuan on 2020/8/15.
//

#include <catch.hpp>
#include <graph/core/dsl/graph_node.h>
#include <graph/core/dsl/sub_graph.h>
#include <nano-caf/core/actor/behavior_based_actor.h>
#include <map>
#include <unordered_map>
#include <graph/core/msgs/graph_msgs.h>


struct root_1 : graph_dsl::root_signature{
   constexpr static auto root_id = 0;
};

struct root_2 : graph_dsl::root_signature{
   constexpr static auto root_id = 1;
};

struct node_3 : graph_dsl::node_signature{
   constexpr static auto id = 3;
};

struct node_4 : graph_dsl::node_signature{
   constexpr static auto id = 4;
};



struct node_5 : graph_dsl::node_signature{
   constexpr static auto id = 5;
};
struct node_6 : graph_dsl::node_signature{
   constexpr static auto id = 6;
};
struct node_7 : graph_dsl::node_signature{
   constexpr static auto id = 7;
};
struct node_8 : graph_dsl::node_signature{
   constexpr static auto id = 8;
};

struct port_1 {
   constexpr static graph_dsl::port_id_t root_port_id = 1;
   constexpr static graph_dsl::port_format format{};
   static auto get_port_format(GRAPH_DSL_NS::graph_context&) -> const graph_dsl::port_format& {
      return format;
   }
};
struct port_2 {
   constexpr static graph_dsl::port_id_t root_port_id = 2;
   constexpr static graph_dsl::port_format format{};
   static auto get_port_format(GRAPH_DSL_NS::graph_context&) -> const graph_dsl::port_format& {
      return format;
   }
};
struct port_3 {
   constexpr static graph_dsl::port_id_t root_port_id = 3;
   constexpr static graph_dsl::port_format format{};
   static auto get_port_format(GRAPH_DSL_NS::graph_context&) -> const graph_dsl::port_format& {
      return format;
   }
};
struct port_4 {
   constexpr static graph_dsl::port_id_t root_port_id = 4;
   constexpr static graph_dsl::port_format format{};
   static auto get_port_format(GRAPH_DSL_NS::graph_context&) -> const graph_dsl::port_format& {
      return format;
   }
};
struct port_5 {
   constexpr static graph_dsl::port_format format{};
   static auto get_port_format(GRAPH_DSL_NS::graph_context&) -> const graph_dsl::port_format& {
      return format;
   }
};
struct port_6 {
   constexpr static graph_dsl::port_format format{};
   static auto get_port_format(GRAPH_DSL_NS::graph_context&) -> const graph_dsl::port_format& {
      return format;
   }
};
struct port_7 {
   constexpr static graph_dsl::port_format format{};
   static auto get_port_format(GRAPH_DSL_NS::graph_context&) -> const graph_dsl::port_format& {
      return format;
   }
};
struct port_8 {
   constexpr static graph_dsl::port_format format{};
   static auto get_port_format(GRAPH_DSL_NS::graph_context&) -> const graph_dsl::port_format& {
      return format;
   }
};
struct port_9 {
   constexpr static graph_dsl::port_format format{};
   static auto get_port_format(GRAPH_DSL_NS::graph_context&) -> const graph_dsl::port_format& {
      return format;
   }
};

struct cond_1 {
   auto operator()(GRAPH_DSL_NS::graph_context&) const -> GRAPH_DSL_NS::result_t<bool> {
      return true;
   }
};

struct cond_2 {
   auto operator()(GRAPH_DSL_NS::graph_context&) const -> GRAPH_DSL_NS::result_t<bool> {
      return false;
   }
};
namespace {

   using root_node =
   __g_NODE(root_1
         , (port_1) -> node_8
         , (port_2) -> __g_MAYBE(cond_2, node_3)
         , (port_3) -> __g_EITHER(cond_1, node_8, node_3)
         , (port_4) -> __g_FORK(node_5, node_4, __g_MAYBE(cond_2, node_8)));

   using grap_def = __g_SUB_GRAPH(
   ( root_1
         , (port_1) -> node_8
         , (port_2) -> __g_MAYBE(cond_2, node_3)
         , (port_3) -> __g_EITHER(cond_1, node_8, node_3)
         , (port_4) -> __g_FORK(node_5, node_4, __g_MAYBE(cond_2, node_8))),
   ( root_2
         , (port_1) -> node_7 ),
   ( node_5
         , (port_5) -> node_8
         , (port_6) -> __g_FORK(node_4, __g_MAYBE(cond_2, node_3))),
   ( node_3
         , (port_7) -> node_4
         , (port_8) -> __g_FORK(node_8, node_6)
         , (port_9) -> node_7));

   template<typename T>
   struct S;

   TEST_CASE("graph_node") {
      static_assert(holo::tuple_t<node_8, node_3, node_5, node_4> == root_node::direct_decedents);
   }

   TEST_CASE("graph_desc") {
      static_assert(holo::tuple_t<
         GRAPH_DSL_NS::node_trait<node_5, GRAPH_DSL_NS::node_category::Intermediate>,
         GRAPH_DSL_NS::node_trait<node_3, GRAPH_DSL_NS::node_category::Intermediate>,
         GRAPH_DSL_NS::node_trait<node_8, GRAPH_DSL_NS::node_category::Leaf>,
         GRAPH_DSL_NS::node_trait<node_4, GRAPH_DSL_NS::node_category::Leaf>,
         GRAPH_DSL_NS::node_trait<node_6, GRAPH_DSL_NS::node_category::Leaf>,
         GRAPH_DSL_NS::node_trait<node_7, GRAPH_DSL_NS::node_category::Leaf>> == grap_def::all_sorted_nodes);
   }
}