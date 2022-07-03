//
// Created by Darwin Yuan on 2020/8/16.
//

#include <graph/core/dsl/graph_node.h>
#include <graph/core/dsl/sub_graph.h>
#include <graph/core/dsl/Graph.h>
#include <graph/core/dsl/graph_roots.h>
#include <graph/core/dsl/sub_graph_selector.h>
#include <nano-caf/actor/Spawn.h>
#include <nano-caf/actor/Actor.h>
#include <iostream>
#include <map>

struct image_buf {
   char buf[1024];
};

CAF_def_message(image_buf_msg_1, (buf, std::shared_ptr<const image_buf>));
CAF_def_message(image_buf_msg_2, (buf, std::shared_ptr<const image_buf>));

struct intermediate_actor : nano_caf::Actor {
   intermediate_actor(int node_id, std::unique_ptr<GRAPH_DSL_NS::ActorPorts> ports)
      : node_id_(node_id)
      , ports_(std::move(ports)) {
      std::cout << node_id_ << ": intermediate created ----------------------> " << std::endl;
   }

   ~intermediate_actor() {
      std::cout << node_id_ << ": intermediate destroyed <----------------------" << std::endl;
   }

   nano_caf::Behavior GetBehavior() {
      return {
         [this](GRAPH_DSL_NS::PortsUpdateMsg::Atom, std::unique_ptr<GRAPH_DSL_NS::ActorPorts> ports) {
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
         [this](nano_caf::ExitMsg::Atom, nano_caf::ExitReason) {
            std::cout << node_id_ << ": intermediate exit" << std::endl;
         }
      };
   }

   template <typename MSG>
   void forward(const MSG& msg) {
      for(auto& [port, targets] : *ports_) {
         targets.Send<MSG>(msg);
      }
   }

private:
   std::unique_ptr<GRAPH_DSL_NS::ActorPorts> ports_;
   int node_id_;
};

struct leaf_actor : nano_caf::Actor {
   leaf_actor(int node_id) : node_id_(node_id) {
      std::cout << node_id_ << ": leaf created ----------------------> " << std::endl;
   }

   ~leaf_actor() {
      std::cout << node_id_ << ": leaf destroyed <-------------------- " << std::endl;
   }

   nano_caf::Behavior GetBehavior() {
      return {
         [this](const image_buf_msg_1&) {
            std::cout << node_id_ << ": leaf image buf 1 received : " << ++counter << std::endl;
         },
         [this](const image_buf_msg_2&) {
            std::cout << node_id_ << ": leaf image buf 2 received : " << ++counter << std::endl;
         },
         [this](nano_caf::ExitMsg::Atom, nano_caf::ExitReason) {
            std::cout << node_id_ << ": leaf exit" << std::endl;
         }
      };
   }

   int counter{0};
   int node_id_;
};

struct root_actor : nano_caf::Actor {
   root_actor(int id, std::unique_ptr<graph_dsl::ActorPorts> ports)
      : id_(id)
      , ports_{std::move(ports)} {
      std::cout << id_ << ": root created" << std::endl;
   }

   ~root_actor() {
      std::cout << id_ << ": root destroyed" << std::endl;
   }

   nano_caf::Behavior GetBehavior() {
      return {
         [this](graph_dsl::PortsUpdateMsg::Atom, std::unique_ptr<graph_dsl::ActorPorts> ports) {
            ports_ = std::move(ports);
            std::cout << id_ << ": root ports updated" << std::endl;
         },
         [this](const image_buf_msg_1& msg) {
            forward(msg);
         },
         [this](const image_buf_msg_2& msg) {
            forward(msg);
         },
         [this](nano_caf::ExitMsg::Atom, nano_caf::ExitReason) {
            std::cout << id_ << ": root exit" << std::endl;
         }
      };
   }

   template<typename MSG>
   void forward(const MSG& msg) {
      for(auto& [port, handles] : *ports_) {
         handles.Send<MSG>(msg);
      }
   }

private:
   std::unique_ptr<graph_dsl::ActorPorts> ports_{};
   int id_;
};

struct root_0 : graph_dsl::RootSignature {
   constexpr static auto id = 0;
   template<typename ... Args>
   static auto spawn(Args&& ... args) -> nano_caf::ActorHandle {
      return nano_caf::Spawn<root_actor>(id, std::forward<Args>(args)...);
   }
};

struct root_1 : graph_dsl::RootSignature {
   constexpr static auto id = 1;
   template<typename ... Args>
   static auto spawn(Args&& ... args) -> nano_caf::ActorHandle {
      return nano_caf::Spawn<root_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_3 : graph_dsl::NodeSignature{
   constexpr static auto id = 3;
   template<typename ... Args>
   static auto spawn(Args&& ... args) -> nano_caf::ActorHandle {
      return nano_caf::Spawn<intermediate_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_4 : graph_dsl::NodeSignature{
   constexpr static auto id = 4;

   template<typename ... Args>
   static auto spawn(Args&& ... args) -> nano_caf::ActorHandle {
      return nano_caf::Spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_5 : graph_dsl::NodeSignature{
   constexpr static auto id = 5;

   template<typename ... Args>
   static auto spawn(Args&& ... args) -> nano_caf::ActorHandle {
      return nano_caf::Spawn<intermediate_actor>(id, std::forward<Args>(args)...);
   }
};
struct node_6 : graph_dsl::NodeSignature{
   constexpr static auto id = 6;
   template<typename ... Args>
   static auto spawn(Args&& ... args) -> nano_caf::ActorHandle {
      return nano_caf::Spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};
struct node_7 : graph_dsl::NodeSignature{
   constexpr static auto id = 7;
   template<typename ... Args>
   static auto spawn(Args&& ... args) -> nano_caf::ActorHandle {
      return nano_caf::Spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};
struct node_8 : graph_dsl::NodeSignature{
   constexpr static auto id = 8;

   template<typename ... Args>
   static auto spawn(Args&& ... args) -> nano_caf::ActorHandle {
      return nano_caf::Spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_9 : graph_dsl::NodeSignature{
   constexpr static auto id = 9;

   template<typename ... Args>
   static auto spawn(Args&& ... args) -> nano_caf::ActorHandle {
      return nano_caf::Spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_10 : graph_dsl::NodeSignature{
   constexpr static auto id = 10;

   template<typename ... Args>
   static auto spawn(Args&& ... args) -> nano_caf::ActorHandle {
      return nano_caf::Spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_11: graph_dsl::NodeSignature{
   constexpr static auto id = 11;

   template<typename ... Args>
   static auto spawn(Args&& ... args) -> nano_caf::ActorHandle {
      return nano_caf::Spawn<intermediate_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_12 : graph_dsl::NodeSignature{
   constexpr static auto id = 12;

   template<typename ... Args>
   static auto spawn(Args&& ... args) -> nano_caf::ActorHandle {
      return nano_caf::Spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_13 : graph_dsl::NodeSignature{
   constexpr static auto id = 13;

   template<typename ... Args>
   static auto spawn(Args&& ... args) -> nano_caf::ActorHandle {
      return nano_caf::Spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_14 : graph_dsl::NodeSignature{
   constexpr static auto id = 14;

   template<typename ... Args>
   static auto spawn(Args&& ... args) -> nano_caf::ActorHandle {
      return nano_caf::Spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};

struct node_15 : graph_dsl::NodeSignature{
   constexpr static auto id = 15;

   template<typename ... Args>
   static auto spawn(Args&& ... args) -> nano_caf::ActorHandle {
      return nano_caf::Spawn<leaf_actor>(id, std::forward<Args>(args)...);
   }
};

struct port_1 {
   constexpr static graph_dsl::PortId port_id = 1;
   constexpr static graph_dsl::PortFormat format{};
   static auto GetPortFormat(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
      return format;
   }
};

struct port_2 {
   constexpr static graph_dsl::PortId port_id = 2;
   constexpr static graph_dsl::PortFormat format{};
   static auto GetPortFormat(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
      return format;
   }
};
struct port_3 {
   constexpr static graph_dsl::PortId port_id = 3;
   constexpr static graph_dsl::PortFormat format{};
   static auto GetPortFormat(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
      return format;
   }
};
struct port_4 {
   constexpr static graph_dsl::PortId port_id = 4;
   constexpr static graph_dsl::PortFormat format{};
   static auto GetPortFormat(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
      return format;
   }
};
struct port_5 {
   constexpr static graph_dsl::PortId port_id = 5;
   constexpr static graph_dsl::PortFormat format{};
   static auto GetPortFormat(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
      return format;
   }
};
struct port_6 {
   constexpr static graph_dsl::PortId port_id = 6;
   constexpr static graph_dsl::PortFormat format{};
   static auto GetPortFormat(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
      return format;
   }
};
struct port_7 {
   constexpr static graph_dsl::PortId port_id = 7;
   constexpr static graph_dsl::PortFormat format{};
   static auto GetPortFormat(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
      return format;
   }
};
struct port_8 {
   constexpr static graph_dsl::PortId port_id = 8;
   constexpr static graph_dsl::PortFormat format{};
   static auto GetPortFormat(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
      return format;
   }
};
struct port_9 {
   constexpr static graph_dsl::PortId port_id = 9;
   constexpr static graph_dsl::PortFormat format{};
   static auto GetPortFormat(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
      return format;
   }
};

struct port_10 {
   constexpr static graph_dsl::PortId port_id = 10;
   constexpr static graph_dsl::PortFormat format{};
   static auto GetPortFormat(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
      return format;
   }
};

struct port_11 {
   constexpr static graph_dsl::PortId port_id = 11;
   constexpr static graph_dsl::PortFormat format{};
   static auto GetPortFormat(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
      return format;
   }
};

struct port_12 {
   constexpr static graph_dsl::PortId port_id = 12;
   constexpr static graph_dsl::PortFormat format{};
   static auto GetPortFormat(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
      return format;
   }
};

bool node_condition = false;

struct cond_1 {
    static auto Satisfied(GRAPH_DSL_NS::GraphContext&) -> GRAPH_DSL_NS::result_t<bool> {
      return node_condition;
   }
};

struct cond_2 {
    static auto Satisfied(GRAPH_DSL_NS::GraphContext&) -> GRAPH_DSL_NS::result_t<bool> {
      return !node_condition;
   }
};

bool sub_graph_switch = true;

struct cond_3 {
   static auto Satisfied(GRAPH_DSL_NS::GraphContext&) -> GRAPH_DSL_NS::result_t<bool> {
      return sub_graph_switch;
   }
};

struct cond_4 {
   static auto Satisfied(GRAPH_DSL_NS::GraphContext&) -> GRAPH_DSL_NS::result_t<bool> {
      return !sub_graph_switch;
   }
};

namespace {

   using sub_graph_1 = __g_SUB_GRAPH(
      ( root_0
              , (port_1) -> node_8
              , (port_2) -> __g_MAYBE(cond_2, node_3)
              , (port_3) -> __g_EITHER(cond_1, node_8, node_4)
              , (port_4) -> __g_FORK(node_5, node_4, __g_MAYBE(cond_2, node_8))),
      ( root_1
              , (port_1) -> node_7),
      ( node_5
              , (port_5) -> node_8
              , (port_6) -> __g_FORK(node_4, __g_MAYBE(cond_2, node_3))),
      ( node_3
              , (port_7) -> node_4
              , (port_8) -> __g_FORK(node_8, node_6)
              , (port_9) -> node_7));


   using sub_graph_2 = __g_SUB_GRAPH(
      ( root_0
              , (port_1) -> node_9),
      ( root_1
              , (port_1) -> node_10
              , (port_2) -> __g_MAYBE(cond_2, node_11)
              , (port_3) -> __g_EITHER(cond_1, node_12, node_13)),
      ( node_11
              , (port_10) -> node_12
              , (port_11) -> __g_FORK(node_13, node_14)
              , (port_12) -> node_15));


   using graph = __g_GRAPH(
      (root_0, root_1),
      (cond_3) -> sub_graph_1,
      (cond_4) -> sub_graph_2);
}


using namespace std::chrono_literals;

GRAPH_DSL_NS::DeviceInfo devices[] = {
   {
      .deviceId = 0,
      .isPreview = true
   },
   {
      .deviceId = 1,
      .isPreview = false
   }
};

GRAPH_DSL_NS::RootState root_states {
   .deviceInfo = devices,
   .size = 2
};

int main() {
   nano_caf::ActorSystem::Instance().StartUp(2);

   GRAPH_DSL_NS::GraphContext context;

   context.UpdateRootState(root_states);
   graph g;

   if(auto status = g.Refresh(context); status != GRAPH_DSL_NS::Status::OK) {
      std::cout << "refresh failed" << std::endl;
      return -1;
   }

   auto tid = std::thread([&]{
      for(int i = 0; i<100; i++) {
         auto msg = std::make_unique<const image_buf>();
         if(auto status = g.GetRoot<0>().Send<image_buf_msg_1>(std::move(msg)); status != nano_caf::Status::OK) {
            std::cout << "0 send failed" << std::endl;
         }
         std::this_thread::sleep_for(1s);
         if(auto status = g.GetRoot<1>().Send<image_buf_msg_2>(std::move(msg)); status != nano_caf::Status::OK) {
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

      if(auto status = g.Refresh(context); status != GRAPH_DSL_NS::Status::OK) { return -1; }
   }

   tid.join();
   g.Stop();
   nano_caf::ActorSystem::Instance().Shutdown();

   return 0;
}

