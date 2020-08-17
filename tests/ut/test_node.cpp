//
// Created by Darwin Yuan on 2020/8/15.
//

#include <catch.hpp>
#include <boost/hana.hpp>
#include <graph/core/node_desc.h>
#include <graph/core/subgraph_desc.h>
#include <nano-caf/core/actor/behavior_based_actor.h>
#include <iostream>
#include <map>
#include <unordered_map>

struct image_buf {
   char buf[1024];
};

CAF_def_message(image_buf_msg, (buf, std::shared_ptr<const image_buf>));

struct node_5_actor : nano_caf::behavior_based_actor {
   node_5_actor(std::unique_ptr<GRAPH_DSL_NS::actor_ports> ports) : ports_(std::move(ports)) {
      std::cout << "intermediate created" << std::endl;
   }
   nano_caf::behavior get_behavior() {
      return {
         [&](const image_buf_msg& msg) {
            std::cout << "1 image buf received" << std::endl;
            forward(msg);
         },

         [](nano_caf::exit_msg_atom, nano_caf::exit_reason) {}
      };
   }

   void forward(const image_buf_msg& msg) {
      for(auto& port : *ports_) {
         for(auto& handle : port.actor_handles_) {
            nano_caf::actor_handle(handle).send<image_buf_msg>(msg);
         }
      }
   }

private:
   std::unique_ptr<GRAPH_DSL_NS::actor_ports> ports_;
};

struct node_8_actor : nano_caf::behavior_based_actor {
   node_8_actor() {
      std::cout << "leaf created" << std::endl;
   }
   nano_caf::behavior get_behavior() {
      return {
         [this](image_buf_msg) {
            std::cout << "image buf received : " << ++counter << std::endl;
         },

         [](nano_caf::exit_msg_atom, nano_caf::exit_reason) {}
      };
   }

   int counter{};
};

struct node_0_actor : nano_caf::behavior_based_actor {
   node_0_actor() {
      std::cout << "root created" << std::endl;
   }
   nano_caf::behavior get_behavior() {
      return {
         [this](const graph_dsl::subgraph_connect_msg& msg) {
            std::cout << "subgraph_connect_msg" << std::endl;
            for(auto& port : *msg.ports) {
               for(auto& handle : port.handles) {
                  ports_[port.port_id].push_back(std::move(handle));
               }
            }

            send();
         },
         [this](image_buf_msg_atom, const std::shared_ptr<const image_buf>& buf) {
            //std::cout << "msg" << std::endl;
            send();
         },
         [](nano_caf::exit_msg_atom, nano_caf::exit_reason) {}
      };
   }

   void send() {
      for(auto& [port, handles] : ports_) {
         for(auto handle : handles) {
            auto msg = std::make_shared<const image_buf>();
            if(handle.send<image_buf_msg>(msg) != nano_caf::status_t::ok) {
               std::cout << "send failed" << std::endl;
            }
         }
      }
   }

private:
   std::unordered_map<GRAPH_DSL_NS::port_id_t, std::vector<nano_caf::actor_handle>> ports_{};
};

struct node_1 : graph_dsl::node_signature{
   constexpr static auto root_id = 0;
   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_0_actor>(std::forward<Args>(args)...);
   }
};

struct node_2 : graph_dsl::node_signature{
   constexpr static auto root_id = 1;
   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_0_actor>(std::forward<Args>(args)...);
   }
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
   __g_ROOT(node_1
         , __g_PORT(port_1) -> node_8
         , __g_PORT(port_2) -> __g_MAYBE(cond_2, node_3)
         , __g_PORT(port_3) -> __g_EITHER(cond_1, node_8, node_3)
         , __g_PORT(port_4) -> __g_FORK(node_5, node_4, __g_MAYBE(cond_2, node_8)));

   using grap_def = __sub_graph(
   __g_ROOT(node_1
         , __g_PORT(port_1) -> node_8
         , __g_PORT(port_2) -> __g_MAYBE(cond_2, node_3)
         , __g_PORT(port_3) -> __g_EITHER(cond_1, node_8, node_3)
         , __g_PORT(port_4) -> __g_FORK(node_5, node_4, __g_MAYBE(cond_2, node_8))),
   __g_ROOT(node_2
         , __g_PORT(port_1) -> node_7 ),
   __g_NODE(node_5
         , __g_PORT(port_5) -> node_8
         , __g_PORT(port_6) -> __g_FORK(node_4, __g_MAYBE(cond_2, node_3))),
   __g_NODE(node_3
         , __g_PORT(port_7) -> node_4
         , __g_PORT(port_8) -> __g_FORK(node_8, node_6)
         , __g_PORT(port_9) -> node_7));

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
}