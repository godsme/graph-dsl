//
// Created by Darwin Yuan on 2020/8/15.
//

#include <catch.hpp>
#include <boost/hana.hpp>
#include <graph/core/node_desc.h>
#include <graph/core/graph_desc.h>
#include <iostream>

struct node_1 : graph_dsl::node_signature{};
struct node_2 : graph_dsl::node_signature{};
struct node_3 : graph_dsl::node_signature{};
struct node_4 : graph_dsl::node_signature{};
struct node_5 : graph_dsl::node_signature{};
struct node_6 : graph_dsl::node_signature{};
struct node_7 : graph_dsl::node_signature{};

struct port_1 {};
struct port_2 {};
struct port_3 {};
struct port_4 {};
struct port_5 {};
struct port_6 {};
struct port_7 {};
struct port_8 {};
struct port_9 {};

struct cond_1 {};
namespace {

   using node_def =
   __node( node_1
         , __port(port_1) -> node_2
         , __port(port_2) -> __maybe(cond_1, node_3)
         , __port(port_3) -> __exclusive(cond_1, node_2, node_3)
         , __port(port_4) -> __fork(node_4, node_2, node_5));

   using node_def_2 =
   __node( node_5
         , __port(port_5) -> node_2
         , __port(port_6) -> __fork(node_4, node_3));

   using node_def_3 =
   __node( node_3
         , __port(port_7) -> node_4
         , __port(port_8) -> __fork(node_2, node_6)
         , __port(port_9) -> node_7);

   using grap_def = __graph(node_def, node_def_2, node_def_3);

   TEST_CASE("node_desc") {
      auto result = node_def::direct_decedents;
      static_assert(result == boost::hana::tuple_t<node_2, node_3, node_4, node_5>);
   }

   template<typename T> struct S;


   TEST_CASE("graph_desc") {
      auto result = grap_def::all_sorted_nodes;
      BOOST_HANA_CONSTANT_CHECK(boost::hana::tuple_t<node_1, node_5, node_3, node_2,node_4, node_6, node_7> == result);
   }
}