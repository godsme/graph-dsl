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
#include <unordered_map>

struct image_buf {
   char buf[1024];
};

CAF_def_message(image_buf_msg_1, (buf, std::shared_ptr<const image_buf>));
CAF_def_message(image_buf_msg_2, (buf, std::shared_ptr<const image_buf>));

struct node_5_actor : nano_caf::behavior_based_actor {
   node_5_actor(int node_id, std::unique_ptr<GRAPH_DSL_NS::actor_ports> ports)
      : node_id_(node_id), ports_(std::move(ports)) {
      std::cout << node_id_ << ": intermediate created" << std::endl;
   }
   ~node_5_actor() {
      std::cout << node_id_ << ": intermediate destroyed" << std::endl;
   }
   nano_caf::behavior get_behavior() {
      return {
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
   std::unique_ptr<GRAPH_DSL_NS::actor_ports> ports_;
   int node_id_;
};

struct node_8_actor : nano_caf::behavior_based_actor {
   node_8_actor(int node_id) : node_id_(node_id) {
      std::cout << node_id_ << ": leaf created" << std::endl;
   }
   ~node_8_actor() {
      std::cout << node_id_ << ": leaf destroyed" << std::endl;
   }
   nano_caf::behavior get_behavior() {
      return {
         [this](image_buf_msg_1) {
            std::cout << node_id_ << ": leaf image buf 1 received : " << ++counter << std::endl;
         },
         [this](image_buf_msg_2) {
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

struct node_0_actor : nano_caf::behavior_based_actor {
   node_0_actor(int id) : id_(id) {
      std::cout << id_ << ": root created" << std::endl;
   }
   ~node_0_actor() {
      std::cout << id_ << ": root destroyed" << std::endl;
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
         },
         [this](const graph_dsl::subgraph_disconnect_msg& msg) {
            std::cout << "subgraph_disconnect_msg ----------- " << std::endl;
            for(auto& port : *msg.ports) {
               for(auto& handle : port.handles) {
                  auto result = std::find(ports_[port.port_id].begin(), ports_[port.port_id].end(), handle);
                  if(result != ports_[port.port_id].end()) {
                     ports_[port.port_id].erase(result);
                  }
               }
            }
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
      for(auto& [port, handles] : ports_) {
         for(auto handle : handles) {
            if(handle.send<MSG>(msg) != nano_caf::status_t::ok) {
               std::cout << id_ << ": send failed" << std::endl;
            }
         }
      }
   }

private:
   std::unordered_map<GRAPH_DSL_NS::port_id_t, std::vector<nano_caf::actor_handle>> ports_{};
   int id_;
};

struct node_1 : graph_dsl::node_signature{
   constexpr static auto id = 1;
   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_0_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_2 : graph_dsl::node_signature{
   constexpr static auto id = 2;
   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_0_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_3 : graph_dsl::node_signature{
   constexpr static auto id = 3;
   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_5_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_4 : graph_dsl::node_signature{
   constexpr static auto id = 4;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_8_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_5 : graph_dsl::node_signature{
   constexpr static auto id = 5;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_5_actor>(id, std::forward<Args>(args)...);
   }
};
struct node_6 : graph_dsl::node_signature{
   constexpr static auto id = 6;
   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_8_actor>(id, std::forward<Args>(args)...);
   }
};
struct node_7 : graph_dsl::node_signature{
   constexpr static auto id = 7;
   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_8_actor>(id, std::forward<Args>(args)...);
   }
};
struct node_8 : graph_dsl::node_signature{
   constexpr static auto id = 8;

   template<typename ... Args>
   static auto spawn(GRAPH_DSL_NS::graph_context& context, Args&& ... args) -> nano_caf::actor_handle {
      return context.get_actor_context().spawn<node_8_actor>(id, std::forward<Args>(args)...);
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

bool condition = false;

struct cond_1 {
   auto operator()(GRAPH_DSL_NS::graph_context&) const -> GRAPH_DSL_NS::result_t<bool> {
      return condition;
   }
};

struct cond_2 {
   auto operator()(GRAPH_DSL_NS::graph_context&) const -> GRAPH_DSL_NS::result_t<bool> {
      return !condition;
   }
};

struct cond_3 {
   auto operator()(GRAPH_DSL_NS::graph_context&) const -> GRAPH_DSL_NS::result_t<bool> {
      return !condition;
   }
};

struct cond_4 {
   auto operator()(GRAPH_DSL_NS::graph_context&) const -> GRAPH_DSL_NS::result_t<bool> {
      return !condition;
   }
};

namespace {

   using graph_def = __sub_graph(
   __g_ROOT(node_1
              , __g_PORT(port_1) -> node_8
              , __g_PORT(port_2) -> __g_MAYBE(cond_2, node_3)
              , __g_PORT(port_3) -> __g_EITHER(cond_1, node_8, node_4)
              , __g_PORT(port_4) -> __g_FORK(node_5, node_4, __g_MAYBE(cond_2, node_8))),
   __g_ROOT(node_2
              , __g_PORT(port_1) -> node_7),
   __g_NODE(node_5
              , __g_PORT(port_5) -> node_8
              , __g_PORT(port_6) -> __g_FORK(node_4, __g_MAYBE(cond_2, node_3))),
   __g_NODE(node_3
              , __g_PORT(port_7) -> node_4
              , __g_PORT(port_8) -> __g_FORK(node_8, node_6)
              , __g_PORT(port_9) -> node_7));


   using graph = __g_GRAPH(
      __g_ROOTS(node_1, node_2),
      __g_WHEN(cond_3) -> graph_def);
}


using namespace std::chrono_literals;

int main() {
   nano_caf::actor_system actor_system;
   actor_system.start(2);
   using roots_type = GRAPH_DSL_NS::root_nodes<node_1, node_2>;
   roots_type roots;

   GRAPH_DSL_NS::graph_context context{actor_system, roots};

   if(auto status = std::get<0>(roots).start(context); status != GRAPH_DSL_NS::status_t::Ok) { return -1; }
   if(auto status = std::get<1>(roots).start(context); status != GRAPH_DSL_NS::status_t::Ok) { return -1; }

   graph_def::by_roots<roots_type> sub_graph;
   if(auto status = sub_graph.build(context); status != GRAPH_DSL_NS::status_t::Ok) { return -1; }
   if(auto status = sub_graph.start(context); status != GRAPH_DSL_NS::status_t::Ok) { return -1; }
   sub_graph.cleanup(context);

   std::cout << actor_system.get_num_of_actors() << std::endl;
   std::cout.flush();

   auto tid = std::thread([&]{
      for(int i = 0; i<100; i++) {
         auto msg = std::make_shared<const image_buf>();
         std::get<0>(roots).get_handle().send<image_buf_msg_1>(msg);
         std::this_thread::sleep_for(33ms);
         std::get<1>(roots).get_handle().send<image_buf_msg_2>(msg);
         std::this_thread::sleep_for(33ms);
      }
   });

//   for(int i=0; i<10; i++) {
//      condition = !condition;
//      std::this_thread::sleep_for(std::chrono::seconds {10});
//      if(auto status = sub_graph.build(context); status != GRAPH_DSL_NS::status_t::Ok) { return -1; }
//      if(auto status = sub_graph.start(context); status != GRAPH_DSL_NS::status_t::Ok) { return -1; }
//      sub_graph.cleanup(context);
//   }

   tid.join();

   std::get<0>(roots).stop();
   std::get<1>(roots).stop();

   sub_graph.stop(context);

   actor_system.shutdown();

   return 0;
}

