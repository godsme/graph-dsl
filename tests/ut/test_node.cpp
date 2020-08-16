//
// Created by Darwin Yuan on 2020/8/15.
//

#include <catch.hpp>
#include <boost/hana.hpp>
#include <graph/core/node_desc.h>
#include <graph/core/subgraph_desc.h>
#include <nano-caf/core/actor/behavior_based_actor.h>
#include <iostream>

struct image_buf {
   char buf[1024];
};

CAF_def_message(image_buf_msg, (buf, std::shared_ptr<const image_buf>));

struct node_5_actor : nano_caf::behavior_based_actor {
   node_5_actor(std::shared_ptr<GRAPH_DSL_NS::actor_ports> ports) : ports_(ports) {
      std::cout << "created" << std::endl;
   }
   nano_caf::behavior get_behavior() {
      return {
         [&](const image_buf_msg& msg) {
            forward(msg);
         },

         [](nano_caf::exit_msg_atom, nano_caf::exit_reason) {}
      };
   }

   void forward(const image_buf_msg& msg) {
      for(auto& port : *ports_) {
         for(auto& handle : port.actor_handles_) {
            handle.send<image_buf_msg>(msg);
         }
      }
   }

private:
   std::shared_ptr<GRAPH_DSL_NS::actor_ports> ports_;
};

struct node_8_actor : nano_caf::behavior_based_actor {
   node_8_actor() {
      std::cout << "leaf created" << std::endl;
   }
   nano_caf::behavior get_behavior() {
      return {
         [](image_buf_msg_atom, const std::shared_ptr<const image_buf>& buf) {
            std::cout << "image buf received" << std::endl;
         },

         [](nano_caf::exit_msg_atom, nano_caf::exit_reason) {}
      };
   }
};

struct node_1 : graph_dsl::node_signature{
   constexpr static auto id = 1;
};
struct node_2 : graph_dsl::node_signature{
   constexpr static auto id = 2;
};
struct node_3 : graph_dsl::node_signature{
   constexpr static auto id = 3;
   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_5_actor>(std::forward<Args>(args)...);
   }
};
struct node_4 : graph_dsl::node_signature{
   constexpr static auto id = 4;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_8_actor>(std::forward<Args>(args)...);
   }
};



struct node_5 : graph_dsl::node_signature{
   constexpr static auto id = 5;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_5_actor>(std::forward<Args>(args)...);
   }
};
struct node_6 : graph_dsl::node_signature{
   constexpr static auto id = 6;
   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_8_actor>(std::forward<Args>(args)...);
   }
};
struct node_7 : graph_dsl::node_signature{
   constexpr static auto id = 7;
   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_8_actor>(std::forward<Args>(args)...);
   }
};
struct node_8 : graph_dsl::node_signature{
   constexpr static auto id = 8;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_8_actor>(std::forward<Args>(args)...);
   }
};

struct port_1 {
   constexpr static graph_dsl::port_format format{};
   static auto get_port_format(GRAPH_DSL_NS::graph_context&) -> const graph_dsl::port_format& {
      return format;
   }
};
struct port_2 {
   constexpr static graph_dsl::port_format format{};
   static auto get_port_format(GRAPH_DSL_NS::graph_context&) -> const graph_dsl::port_format& {
      return format;
   }
};
struct port_3 {
   constexpr static graph_dsl::port_format format{};
   static auto get_port_format(GRAPH_DSL_NS::graph_context&) -> const graph_dsl::port_format& {
      return format;
   }
};
struct port_4 {
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
   __root( node_1
         , __port(port_1) -> node_8
         , __port(port_2) -> __maybe(cond_2, node_3)
         , __port(port_3) -> __either(cond_1, node_8, node_3)
         , __port(port_4) -> __fork(node_5, node_4, __maybe(cond_2, node_8)));

   using grap_def = __sub_graph(
   __root( node_1
      , __port(port_1) -> node_8
      , __port(port_2) -> __maybe(cond_2, node_3)
      , __port(port_3) -> __either(cond_1, node_8, node_3)
      , __port(port_4) -> __fork(node_5, node_4, __maybe(cond_2, node_8))),
   __root( node_2
         , __port(port_1) -> node_7 ),
   __node( node_5
         , __port(port_5) -> node_8
         , __port(port_6) -> __fork(node_4, __maybe(cond_2, node_3))),
   __node( node_3
         , __port(port_7) -> node_4
         , __port(port_8) -> __fork(node_8, node_6)
         , __port(port_9) -> node_7));

   template<typename T>
   struct S;

   TEST_CASE("node_desc") {
      static_assert(boost::hana::tuple_t<node_8, node_3, node_5, node_4> == root_node::direct_decedents);
   }

   TEST_CASE("graph_desc") {
      static_assert(boost::hana::tuple_t<
         GRAPH_DSL_NS::node_trait<node_1, GRAPH_DSL_NS::node_category::Root>,
         GRAPH_DSL_NS::node_trait<node_2, GRAPH_DSL_NS::node_category::Root>,
         GRAPH_DSL_NS::node_trait<node_5, GRAPH_DSL_NS::node_category::Intermediate>,
         GRAPH_DSL_NS::node_trait<node_3, GRAPH_DSL_NS::node_category::Intermediate>,
         GRAPH_DSL_NS::node_trait<node_8, GRAPH_DSL_NS::node_category::Leaf>,
         GRAPH_DSL_NS::node_trait<node_4, GRAPH_DSL_NS::node_category::Leaf>,
         GRAPH_DSL_NS::node_trait<node_6, GRAPH_DSL_NS::node_category::Leaf>,
         GRAPH_DSL_NS::node_trait<node_7, GRAPH_DSL_NS::node_category::Leaf>> == grap_def::all_sorted_nodes);
   }

   TEST_CASE("graph_desc build") {
      nano_caf::actor_system actor_system;
      actor_system.start(1);
      GRAPH_DSL_NS::graph_context context{actor_system};
      grap_def graph;
      REQUIRE(GRAPH_DSL_NS::status_t::Ok == graph.build(context));
      REQUIRE(GRAPH_DSL_NS::status_t::Ok == graph.build(context));
      REQUIRE(GRAPH_DSL_NS::status_t::Ok == graph.start(context));

      graph.dump();

      std::cout << actor_system.get_num_of_actors() << std::endl;
      std::cout.flush();
      //actor_system.shutdown();
   }


}