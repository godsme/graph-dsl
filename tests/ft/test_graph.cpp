//
// Created by Darwin Yuan on 2020/8/16.
//

#include <graph/core/dsl/graph_node.h>
#include <graph/core/dsl/sub_graph.h>
#include <graph/core/dsl/graph.h>
#include <graph/core/dsl/graph_roots.h>
#include <graph/core/dsl/sub_graph_selector.h>
#include <nano-caf/core/actor/behavior_based_actor.h>
#include <iostream>
#include <map>

struct image_buf {
   char buf[1024];
};

CAF_def_message(image_buf_msg_1, (buf, std::shared_ptr<const image_buf>));
CAF_def_message(image_buf_msg_2, (buf, std::shared_ptr<const image_buf>));

struct intermediate_actor : nano_caf::behavior_based_actor {
   intermediate_actor(int node_id, std::unique_ptr<GRAPH_DSL_NS::actor_ports> ports)
      : node_id_(node_id)
      , ports_(std::move(ports)) {
      std::cout << node_id_ << ": intermediate created ----------------------> " << std::endl;
   }

   ~intermediate_actor() {
      std::cout << node_id_ << ": intermediate destroyed <----------------------" << std::endl;
   }

   nano_caf::behavior get_behavior() {
      return {
         [this](GRAPH_DSL_NS::subgraph_ports_update_msg_atom, std::shared_ptr<GRAPH_DSL_NS::actor_ports> ports) {
            ports_ = std::move(ports);
         },
         [this](const image_buf_msg_1& msg) {
            std::cout << node_id_ << ": intermediate image buf 1 received" << std::endl;
            forward(msg);
         },
         [this](const image_buf_msg_2& msg) {
            std::cout << node_id_ << ": intermediate image buf 2 received" << std::endl;
            forward(msg);
         },
         [this](nano_caf::exit_msg_atom, nano_caf::exit_reason) {
            std::cout << node_id_ << ": intermediate exit" << std::endl;
         }
      };
   }

   template <typename MSG>
   void forward(const MSG& msg) {
      for(auto& port : *ports_) {
         for(auto& handle : port.actor_handles_) {
            nano_caf::actor_handle(handle).send<MSG>(msg);
         }
      }
   }

private:
   std::shared_ptr<GRAPH_DSL_NS::actor_ports> ports_;
   int node_id_;
};

struct leaf_actor : nano_caf::behavior_based_actor {
   leaf_actor(int node_id) : node_id_(node_id) {
      std::cout << node_id_ << ": leaf created ----------------------> " << std::endl;
   }

   ~leaf_actor() {
      std::cout << node_id_ << ": leaf destroyed <-------------------- " << std::endl;
   }

   nano_caf::behavior get_behavior() {
      return {
         [this](const image_buf_msg_1&) {
            std::cout << node_id_ << ": leaf image buf 1 received : " << ++counter << std::endl;
         },
         [this](const image_buf_msg_2&) {
            std::cout << node_id_ << ": leaf image buf 2 received : " << ++counter << std::endl;
         },
         [this](nano_caf::exit_msg_atom, nano_caf::exit_reason) {
            std::cout << node_id_ << ": leaf exit" << std::endl;
         }
      };
   }

   int counter{0};
   int node_id_;
};

struct root_actor : nano_caf::behavior_based_actor {
   root_actor(int id, std::shared_ptr<graph_dsl::root_ports> ports)
      : id_(id)
      , ports_{std::move(ports)} {
      std::cout << id_ << ": root created" << std::endl;
   }

   ~root_actor() {
      std::cout << id_ << ": root destroyed" << std::endl;
   }

   nano_caf::behavior get_behavior() {
      return {
         [this](graph_dsl::root_ports_update_msg_atom, std::shared_ptr<graph_dsl::root_ports> ports) {
            ports_ = std::move(ports);
            std::cout << id_ << ": root ports updated" << std::endl;
         },
         [this](const image_buf_msg_1& msg) {
            forward(msg);
         },
         [this](const image_buf_msg_2& msg) {
            forward(msg);
         },
         [this](nano_caf::exit_msg_atom, nano_caf::exit_reason) {
            std::cout << id_ << ": root exit" << std::endl;
         }
      };
   }

   template<typename MSG>
   void forward(const MSG& msg) {
      for(auto& [port, handles] : *ports_) {
         for(auto handle : handles) {
            if(handle.send<MSG>(msg) != nano_caf::status_t::ok) {
               std::cout << id_ << ": send failed" << std::endl;
            }
         }
      }
   }

private:
   std::shared_ptr<graph_dsl::root_ports> ports_{};
   int id_;
};

struct node_1 : graph_dsl::node_signature{
   constexpr static auto id = 1;
   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<root_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_2 : graph_dsl::node_signature{
   constexpr static auto id = 2;
   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<root_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_3 : graph_dsl::node_signature{
   constexpr static auto id = 3;
   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<intermediate_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_4 : graph_dsl::node_signature{
   constexpr static auto id = 4;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_5 : graph_dsl::node_signature{
   constexpr static auto id = 5;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<intermediate_actor>(id, std::forward<Args>(args)...);
   }
};
struct node_6 : graph_dsl::node_signature{
   constexpr static auto id = 6;
   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};
struct node_7 : graph_dsl::node_signature{
   constexpr static auto id = 7;
   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};
struct node_8 : graph_dsl::node_signature{
   constexpr static auto id = 8;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_9 : graph_dsl::node_signature{
   constexpr static auto id = 9;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_10 : graph_dsl::node_signature{
   constexpr static auto id = 10;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_11: graph_dsl::node_signature{
   constexpr static auto id = 11;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<intermediate_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_12 : graph_dsl::node_signature{
   constexpr static auto id = 12;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_13 : graph_dsl::node_signature{
   constexpr static auto id = 13;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_14 : graph_dsl::node_signature{
   constexpr static auto id = 14;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_15 : graph_dsl::node_signature{
   constexpr static auto id = 15;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<leaf_actor>(id, std::forward<Args>(args)...);
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

struct port_10 {
   constexpr static graph_dsl::port_format format{};
   static auto get_port_format(GRAPH_DSL_NS::graph_context&) -> const graph_dsl::port_format& {
      return format;
   }
};

struct port_11 {
   constexpr static graph_dsl::port_format format{};
   static auto get_port_format(GRAPH_DSL_NS::graph_context&) -> const graph_dsl::port_format& {
      return format;
   }
};

struct port_12 {
   constexpr static graph_dsl::port_format format{};
   static auto get_port_format(GRAPH_DSL_NS::graph_context&) -> const graph_dsl::port_format& {
      return format;
   }
};

bool node_condition = false;

struct cond_1 {
   auto operator()(GRAPH_DSL_NS::graph_context&) const -> GRAPH_DSL_NS::result_t<bool> {
      return node_condition;
   }
};

struct cond_2 {
   auto operator()(GRAPH_DSL_NS::graph_context&) const -> GRAPH_DSL_NS::result_t<bool> {
      return !node_condition;
   }
};

bool sub_graph_switch = true;

struct cond_3 {
   auto operator()(GRAPH_DSL_NS::graph_context&) const -> GRAPH_DSL_NS::result_t<bool> {
      return sub_graph_switch;
   }
};

struct cond_4 {
   auto operator()(GRAPH_DSL_NS::graph_context&) const -> GRAPH_DSL_NS::result_t<bool> {
      return !sub_graph_switch;
   }
};

namespace {

   using sub_graph_1 = __g_SUB_GRAPH(
      __g_ROOT( node_1
              , __g_PORT(port_1) -> node_8
              , __g_PORT(port_2) -> __g_MAYBE(cond_2, node_3)
              , __g_PORT(port_3) -> __g_EITHER(cond_1, node_8, node_4)
              , __g_PORT(port_4) -> __g_FORK(node_5, node_4, __g_MAYBE(cond_2, node_8))),
      __g_ROOT( node_2
              , __g_PORT(port_1) -> node_7),
      __g_NODE( node_5
              , __g_PORT(port_5) -> node_8
              , __g_PORT(port_6) -> __g_FORK(node_4, __g_MAYBE(cond_2, node_3))),
      __g_NODE( node_3
              , __g_PORT(port_7) -> node_4
              , __g_PORT(port_8) -> __g_FORK(node_8, node_6)
              , __g_PORT(port_9) -> node_7));


   using sub_graph_2 = __g_SUB_GRAPH(
      __g_ROOT( node_1
              , __g_PORT(port_1) -> node_9),
      __g_ROOT( node_2
              , __g_PORT(port_1) -> node_10
              , __g_PORT(port_2) -> __g_MAYBE(cond_2, node_11)
              , __g_PORT(port_3) -> __g_EITHER(cond_1, node_12, node_13)),
      __g_NODE(node_11
              , __g_PORT(port_10) -> node_12
              , __g_PORT(port_11) -> __g_FORK(node_13, node_14)
              , __g_PORT(port_12) -> node_15));


   using graph = __g_GRAPH(
      __g_ROOTS(node_1, node_2),
      __g_IF(cond_3) -> sub_graph_1,
      __g_IF(cond_4) -> sub_graph_2);
}


using namespace std::chrono_literals;

int test_2() {
   nano_caf::actor_system actor_system;
   actor_system.start(2);

   GRAPH_DSL_NS::graph_context context{actor_system};
   graph g;

   if(auto status = g.refresh(context); status != GRAPH_DSL_NS::status_t::Ok) { return -1; }

   auto tid = std::thread([&]{
      for(int i = 0; i<100; i++) {
         auto msg = std::make_unique<const image_buf>();
         if(auto status = g.get_root<0>().send<image_buf_msg_1>(std::move(msg)); status != nano_caf::status_t::ok) {
            std::cout << "0 send failed" << std::endl;
         }
         std::this_thread::sleep_for(1s);
         if(auto status = g.get_root<1>().send<image_buf_msg_2>(std::move(msg)); status != nano_caf::status_t::ok) {
            std::cout << "1 send failed" << std::endl;
         }

         std::this_thread::sleep_for(1s);
      }
   });

   for(auto i=0; i<40; i++) {
      std::this_thread::sleep_for(5s);

      node_condition = !node_condition;

      if((i > 0) && (i % 10) == 0) {
         sub_graph_switch = !sub_graph_switch;
      }

      if(auto status = g.refresh(context); status != GRAPH_DSL_NS::status_t::Ok) { return -1; }
   }

   tid.join();
   g.stop();
   actor_system.shutdown();

   return 0;
}

int main() {
   return test_2();
}

