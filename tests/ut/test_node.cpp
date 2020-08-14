//
// Created by Darwin Yuan on 2020/8/15.
//

#include <catch.hpp>
#include <boost/hana.hpp>
#include <graph/core/node_desc.h>
#include <iostream>

struct node_1 : graph_dsl::node_signature{};
struct node_2 : graph_dsl::node_signature{};
struct node_3 : graph_dsl::node_signature{};
struct node_4 : graph_dsl::node_signature{};
struct node_5 : graph_dsl::node_signature{};
struct node_6 : graph_dsl::node_signature{};

struct port_1 {};
struct port_2 {};
struct port_3 {};
struct port_4 {};

struct cond_1 {};
namespace {

   using node_def =
   __node( node_1
         , __port(port_1) -> node_2
         , __port(port_2) -> __maybe(cond_1, node_3)
         , __port(port_3) -> __exclusive(cond_1, node_2, node_3)
         , __port(port_4) -> __fork(node_4, node_2, node_5));

   TEST_CASE("node_desc") {
      auto result = node_def::direct_decedents;
      static_assert(result == boost::hana::tuple_t<node_2, node_3, node_4, node_5>);
   }
}