//
// Created by Darwin Yuan on 2020/8/15.
//

#include <catch.hpp>
#include <boost/hana.hpp>
#include <graph/core/node_desc.h>
#include <graph/core/graph_desc.h>
#include <iostream>

struct node_1 : graph_dsl::node_signature{
   constexpr static auto id = 1;
};
struct node_2 : graph_dsl::node_signature{
   constexpr static auto id = 2;
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

struct port_1 {};
struct port_2 {};
struct port_3 {};
struct port_4 {};
struct port_5 {};
struct port_6 {};
struct port_7 {};
struct port_8 {};
struct port_9 {};

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

   using node_def =
   __root( node_1
         , __port(port_1) -> node_2
         , __port(port_2) -> __maybe(cond_2, node_3)
         , __port(port_3) -> __either(cond_1, node_2, node_3)
         , __port(port_4) -> __fork(node_5, node_4, __maybe(cond_2, node_2)));

   using grap_def = __graph(node_def,
   __node( node_5
         , __port(port_5) -> node_2
         , __port(port_6) -> __fork(node_4, __maybe(cond_2, node_3))),
   __node( node_3
         , __port(port_7) -> node_4
         , __port(port_8) -> __fork(node_2, node_6)
         , __port(port_9) -> node_7));

   template<typename T>
   struct S;

   TEST_CASE("node_desc") {
      static_assert(boost::hana::tuple_t<node_2, node_3, node_5, node_4> == node_def::direct_decedents);
   }

   TEST_CASE("graph_desc") {
      static_assert(boost::hana::tuple_t<node_5, node_3, node_2,node_4, node_6, node_7> == grap_def::all_sorted_nodes);
   }

   TEST_CASE("graph_desc build") {
      GRAPH_DSL_NS::graph_context context;
      grap_def graph;
      REQUIRE(GRAPH_DSL_NS::status_t::Ok == graph.build(context));
      REQUIRE(GRAPH_DSL_NS::status_t::Ok == graph.build(context));
      graph.dump();
   }


}