////
//// Created by Darwin Yuan on 2020/8/21.
////
//
//#include <Graph/core/dsl/graph_node.h>
//#include <Graph/core/dsl/sub_graph.h>
//#include <Graph/core/dsl/Graph.h>
//#include <Graph/core/dsl/graph_roots.h>
//#include <Graph/core/dsl/sub_graph_selector.h>
//#include <Graph/core/dsl/multi_device_graph.h>
//#include <nano-caf/actor/Actor.h>
//#include <nano-caf/actor/Spawn.h>
//#include <random>
//#include <map>
//
//struct image_buf {
//   char buf[1024];
//};
//
//CAF_def_message(image_buf_msg_1, (buf, std::shared_ptr<const image_buf>));
//CAF_def_message(image_buf_msg_2, (buf, std::shared_ptr<const image_buf>));
//
//struct intermediate_actor : nano_caf::Actor {
//   intermediate_actor(int node_id, std::unique_ptr<GRAPH_DSL_NS::ActorPorts> ports)
//      : node_id_(node_id)
//      , ports_(std::move(ports)) {
//   }
//
//   ~intermediate_actor() override {
//   }
//
//   nano_caf::Behavior GetBehavior() {
//      return {
//         [this](GRAPH_DSL_NS::PortsUpdateMsg::Atom, std::unique_ptr<GRAPH_DSL_NS::ActorPorts> ports) {
//            ports_ = std::move(ports);
//         },
//         [this](const image_buf_msg_1& msg) {
//            forward(msg);
//         },
//         [this](const image_buf_msg_2& msg) {
//            forward(msg);
//         },
//         [this](nano_caf::ExitMsg::Atom, nano_caf::ExitReason) {
//         }
//      };
//   }
//
//   template <typename MSG>
//   void forward(const MSG& msg) {
//      for(auto& [port, handlers] : *ports_) {
//         handlers.Send<MSG>(msg);
//      }
//   }
//
//private:
//   std::unique_ptr<GRAPH_DSL_NS::ActorPorts> ports_;
//   int counter{0};
//   int node_id_;
//};
//
//struct leaf_actor : nano_caf::Actor {
//   leaf_actor(int node_id) : node_id_(node_id) {
//   }
//
//   ~leaf_actor() override {
//   }
//
//   nano_caf::Behavior GetBehavior() {
//      return {
//         [this](const image_buf_msg_1&) {
//         },
//         [this](const image_buf_msg_2&) {
//         },
//         [this](nano_caf::ExitMsg::Atom, nano_caf::ExitReason) {
//         }
//      };
//   }
//
//   int counter{0};
//   int node_id_;
//};
//
//struct root_actor : nano_caf::Actor {
//   root_actor(int id, std::unique_ptr<graph_dsl::ActorPorts> ports)
//      : id_(id)
//      , ports_{std::move(ports)} {
//   }
//
//   ~root_actor() override {
//   }
//
//   nano_caf::Behavior GetBehavior() {
//      return {
//         [this](graph_dsl::PortsUpdateMsg::Atom, std::unique_ptr<graph_dsl::ActorPorts> ports) {
//            ports_ = std::move(ports);
//         },
//         [this](const image_buf_msg_1& msg) {
//            forward(msg);
//         },
//         [this](const image_buf_msg_2& msg) {
//            forward(msg);
//         },
//         [this](nano_caf::ExitMsg::Atom, nano_caf::ExitReason) {
//         }
//      };
//   }
//
//   template<typename MSG>
//   void forward(const MSG& msg) {
//      for(auto& [port, handles] : *ports_) {
//         handles.Send<MSG>(msg);
//      }
//   }
//
//private:
//   std::unique_ptr<graph_dsl::ActorPorts> ports_{};
//   int id_;
//};
//
//struct root_0 : graph_dsl::root_signature{
//   constexpr static auto Device_Id = 0;
//   constexpr static auto id = 0;
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<root_actor>(id, std::forward<Args>(args)...);
//   }
//};
//
//struct root_1 : graph_dsl::root_signature {
//   constexpr static auto Device_Id = 1;
//   constexpr static auto id = 1;
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<root_actor>(id, std::forward<Args>(args)...);
//   }
//};
//
//struct root_2 : graph_dsl::root_signature {
//   constexpr static auto Device_Id = 2;
//   constexpr static auto id = 2;
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<root_actor>(id, std::forward<Args>(args)...);
//   }
//};
//
//struct root_3 : graph_dsl::root_signature {
//   constexpr static auto Device_Id = 3;
//   constexpr static auto id = 3;
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<root_actor>(id, std::forward<Args>(args)...);
//   }
//};
//
//struct node_4 : graph_dsl::node_signature{
//   constexpr static auto id = 4;
//
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<leaf_actor>(id, std::forward<Args>(args)...);
//   }
//};
//
//struct node_5 : graph_dsl::node_signature{
//   constexpr static auto id = 5;
//
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<intermediate_actor>(id, std::forward<Args>(args)...);
//   }
//};
//struct node_6 : graph_dsl::node_signature{
//   constexpr static auto id = 6;
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<intermediate_actor>(id, std::forward<Args>(args)...);
//   }
//};
//struct node_7 : graph_dsl::node_signature{
//   constexpr static auto id = 7;
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<leaf_actor>(id, std::forward<Args>(args)...);
//   }
//};
//struct node_8 : graph_dsl::node_signature{
//   constexpr static auto id = 8;
//
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<leaf_actor>(id, std::forward<Args>(args)...);
//   }
//};
//
//struct node_9 : graph_dsl::node_signature{
//   constexpr static auto id = 9;
//
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<leaf_actor>(id, std::forward<Args>(args)...);
//   }
//};
//
//struct node_10 : graph_dsl::node_signature{
//   constexpr static auto id = 10;
//
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<leaf_actor>(id, std::forward<Args>(args)...);
//   }
//};
//
//struct node_11: graph_dsl::node_signature{
//   constexpr static auto id = 11;
//
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<intermediate_actor>(id, std::forward<Args>(args)...);
//   }
//};
//
//struct node_12 : graph_dsl::node_signature{
//   constexpr static auto id = 12;
//
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<leaf_actor>(id, std::forward<Args>(args)...);
//   }
//};
//
//struct node_13 : graph_dsl::node_signature{
//   constexpr static auto id = 13;
//
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<leaf_actor>(id, std::forward<Args>(args)...);
//   }
//};
//
//struct node_14 : graph_dsl::node_signature{
//   constexpr static auto id = 14;
//
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<leaf_actor>(id, std::forward<Args>(args)...);
//   }
//};
//
//struct node_15 : graph_dsl::node_signature{
//   constexpr static auto id = 15;
//
//   template<typename ... Args>
//   static auto Spawn( Args&& ... args) -> nano_caf::ActorHandle {
//      return Spawn<leaf_actor>(id, std::forward<Args>(args)...);
//   }
//};
//
//struct port_1 {
//   constexpr static graph_dsl::PortId port_id = 1;
//   constexpr static graph_dsl::PortFormat format{};
//   static auto get_port_format(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
//      return format;
//   }
//};
//
//struct port_2 {
//   constexpr static graph_dsl::PortId port_id = 2;
//   constexpr static graph_dsl::PortFormat format{};
//   static auto get_port_format(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
//      return format;
//   }
//};
//struct port_3 {
//   constexpr static graph_dsl::PortId port_id = 3;
//   constexpr static graph_dsl::PortFormat format{};
//   static auto get_port_format(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
//      return format;
//   }
//};
//struct port_4 {
//   constexpr static graph_dsl::PortId port_id = 4;
//   constexpr static graph_dsl::PortFormat format{};
//   static auto get_port_format(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
//      return format;
//   }
//};
//struct port_5 {
//   constexpr static graph_dsl::PortId port_id = 5;
//   constexpr static graph_dsl::PortFormat format{};
//   static auto get_port_format(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
//      return format;
//   }
//};
//struct port_6 {
//   constexpr static graph_dsl::PortId port_id = 6;
//   constexpr static graph_dsl::PortFormat format{};
//   static auto get_port_format(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
//      return format;
//   }
//};
//struct port_7 {
//   constexpr static graph_dsl::PortId port_id = 7;
//   constexpr static graph_dsl::PortFormat format{};
//   static auto get_port_format(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
//      return format;
//   }
//};
//struct port_8 {
//   constexpr static graph_dsl::PortId port_id = 8;
//   constexpr static graph_dsl::PortFormat format{};
//   static auto get_port_format(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
//      return format;
//   }
//};
//struct port_9 {
//   constexpr static graph_dsl::PortId port_id = 9;
//   constexpr static graph_dsl::PortFormat format{};
//   static auto get_port_format(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
//      return format;
//   }
//};
//
//struct port_10 {
//   constexpr static graph_dsl::PortId port_id = 10;
//   constexpr static graph_dsl::PortFormat format{};
//   static auto get_port_format(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
//      return format;
//   }
//};
//
//struct port_11 {
//   constexpr static graph_dsl::PortId port_id = 11;
//   constexpr static graph_dsl::PortFormat format{};
//   static auto get_port_format(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
//      return format;
//   }
//};
//
//struct port_12 {
//   constexpr static graph_dsl::PortId port_id = 12;
//   constexpr static graph_dsl::PortFormat format{};
//   static auto get_port_format(GRAPH_DSL_NS::GraphContext&) -> const graph_dsl::PortFormat& {
//      return format;
//   }
//};
//
//bool node_condition = false;
//
//struct cond_1 {
//   auto operator()(GRAPH_DSL_NS::GraphContext&) const -> GRAPH_DSL_NS::result_t<bool> {
//      return node_condition;
//   }
//};
//
//struct cond_2 {
//   auto operator()(GRAPH_DSL_NS::GraphContext&) const -> GRAPH_DSL_NS::result_t<bool> {
//      return !node_condition;
//   }
//};
//
//bool sub_graph_switch = true;
//
//struct cond_3 {
//   auto operator()(GRAPH_DSL_NS::GraphContext&) const -> GRAPH_DSL_NS::result_t<bool> {
//      return sub_graph_switch;
//   }
//};
//
//struct cond_4 {
//   auto operator()(GRAPH_DSL_NS::GraphContext&) const -> GRAPH_DSL_NS::result_t<bool> {
//      return sub_graph_switch;
//   }
//};
//
//namespace {
//
//   using sub_graph_1 = __g_SUB_GRAPH(
//      ( root_0
//              , (port_1) -> node_8
//              , (port_2) -> __g_MAYBE(cond_2, node_6)
//              , (port_3) -> __g_EITHER(cond_1, node_8, node_4)
//              , (port_4) -> __g_FORK(node_5, node_4, __g_MAYBE(cond_2, node_8))),
//      ( root_1
//              , (port_1) -> node_7),
//      ( node_5
//              , (port_5) -> node_8
//              , (port_6) -> __g_FORK(node_4, __g_MAYBE(cond_2, node_8))),
//      ( node_6
//              , (port_7) -> node_4
//              , (port_8) -> __g_FORK(node_8, node_9)
//              , (port_9) -> node_7));
//
//
//   using sub_graph_2 = __g_SUB_GRAPH(
//      ( root_1
//              , (port_1) -> node_13),
//      ( root_2
//              , (port_1) -> node_10
//              , (port_2) -> __g_MAYBE(cond_2, node_11)
//              , (port_3) -> __g_EITHER(cond_1, node_12, node_13)),
//      ( root_3
//              , (port_1) -> node_12),
//      ( node_11
//              , (port_10) -> node_12
//              , (port_11) -> __g_FORK(node_13, node_14)
//              , (port_12) -> node_15));
//
//   using Graph = __g_GRAPH(
//      (root_0, root_1, root_2, root_3),
//      (cond_3) -> sub_graph_1,
//      (cond_4) -> sub_graph_2);
//}
//
//
//using namespace std::chrono_literals;
//
//using selector =
//__g_STATE_SELECTOR(
//      (__g_SCENE_MODE(1), __g_COND_1( [1.0, 2.0} ))
//   -> __g_STATE(root_0, __g_PREVIEW(root_1)),
//
//      (__g_SCENE_MODE(1), __g_COND_1( [2.0, 3.0} ))
//   -> __g_STATE(root_1, __g_PREVIEW(root_2)),
//
//      (__g_SCENE_MODE(1), __g_COND_1( [3.0, 4.0} ))
//   -> __g_STATE(root_2, __g_PREVIEW(root_3)),
//
//      (__g_SCENE_MODE(1), __g_COND_1( [4.0, 5.0} ))
//   -> __g_STATE(root_3, __g_PREVIEW(root_1)),
//
//      (__g_SCENE_MODE(1), __g_COND_1( [1.0, 2.0} ), __g_COND_2(1, 5))
//   -> __g_STATE(__g_PREVIEW(root_2), root_1),
//
//      (__g_SCENE_MODE(1), __g_COND_1( [1.5, 2.0} ), __g_COND_2(1, 5), __g_COND_3(3, 5))
//   -> __g_STATE(root_2, __g_PREVIEW(root_1)),
//
//      (__g_SCENE_MODE(1), __g_COND_1( [2.0, 3.0} ), __g_COND_2(1, 5), __g_COND_3(3, 5))
//   -> __g_STATE(root_1, __g_PREVIEW(root_0)),
//
//      (__g_SCENE_MODE(1), __g_COND_1( [2.0, 3.0} ), __g_COND_2( [5, 10} ), __g_COND_3(3, 5))
//   -> __g_STATE(root_1, __g_PREVIEW(root_2)),
//
//      (__g_SCENE_MODE(1), __g_COND_1( [2.0, 3.0} ), __g_COND_2( [5, 10} ), __g_COND_3([5, 10}))
//   -> __g_STATE(root_3, __g_PREVIEW(root_1)),
//
//      (__g_SCENE_MODE(1), __g_COND_1( [3.0, 4.0} ), __g_COND_2( [5, 10} ), __g_COND_3([5, 10}))
//   -> __g_STATE(root_3, __g_PREVIEW(root_2)),
//
//      (__g_SCENE_MODE(1), __g_COND_1( [3.0, 4.0} ), __g_COND_2( [10, 15} ), __g_COND_3([5, 10}))
//   -> __g_STATE(root_2, __g_PREVIEW(root_3)));
//
//using transitions =
//__g_STATE_TRANSITIONS(
//   ((root_0, __g_PREVIEW(root_1)) -> __g_PREVIEW(root_2), root_1),
//   ((__g_PREVIEW(root_2), root_1) -> root_0, __g_PREVIEW(root_1)),
//   ((root_2, __g_PREVIEW(root_1)) -> __g_PREVIEW(root_0), root_1),
//   ((__g_PREVIEW(root_0), root_1) -> root_2, __g_PREVIEW(root_1)),
//   ((__g_PREVIEW(root_0), root_1) -> root_1, __g_PREVIEW(root_2)),
//   ((root_1, __g_PREVIEW(root_2)) -> __g_PREVIEW(root_1), root_3),
//   ((__g_PREVIEW(root_1), root_3) -> __g_PREVIEW(root_2), root_3),
//   ((__g_PREVIEW(root_2), root_3) -> root_2, __g_PREVIEW(root_3)),
//   ((root_2, __g_PREVIEW(root_3)) -> root_1, __g_PREVIEW(root_2)));
//
//using md_graph = GRAPH_DSL_NS::multi_device_graph<Graph, selector, transitions>;
//
//struct environment {
//   [[nodiscard]] auto get_scene() const -> uint8_t {
//      return scene;
//   }
//   [[nodiscard]] auto get_condition_1() const -> double {
//      return condition_1;
//   }
//   [[nodiscard]] auto get_condition_2() const -> unsigned int {
//      return condition_2;
//   }
//   [[nodiscard]] auto get_condition_3() const -> unsigned int {
//      return condition_3;
//   }
//
//   uint8_t scene{1};
//   double condition_1{1.5};
//   unsigned int condition_2{20};
//   unsigned int condition_3{20};
//};
//
//struct hardware_actor : nano_caf::Actor {
//   hardware_actor(md_graph& Graph, int which)
//      : graph_{Graph}, which_msg_{which} {
//   }
//
//   ~hardware_actor() {
//   }
//
//   auto OnInit() -> void {
//      repeat(33ms, [this]{
//         auto msg = std::make_unique<const image_buf>();
//         if(which_msg_) {
//            graph_.get_root<0>().Send<image_buf_msg_1>(std::move(msg));
//            graph_.get_root<2>().Send<image_buf_msg_2>(std::move(msg));
//         } else {
//            graph_.get_root<1>().Send<image_buf_msg_1>(std::move(msg));
//            graph_.get_root<3>().Send<image_buf_msg_2>(std::move(msg));
//         }
//      });
//   }
//
//private:
//   md_graph& graph_;
//   int which_msg_{0};
//};
//
//CAF_def_message(start, (env, environment));
//CAF_def_message(env_change, (env, environment));
//CAF_def_message(meta_change);
//CAF_def_message(switch_done);
//CAF_def_message(stop);
//
//struct session_actor : nano_caf::behavior_based_actor {
//   session_actor() : context(*this) {
//      spdlog::info("session_actor created");
//   }
//
//   ~session_actor() {
//      spdlog::info("session_actor destroyed");
//   }
//
//   auto on_init() -> void override {
//      hw_1 = spawn<hardware_actor>(Graph, 0);
//      hw_2 = spawn<hardware_actor>(Graph, 1);
//   }
//
//   nano_caf::Behavior GetBehavior() override {
//      return {
//         [this](start_atom, const environment& env) {
//            on_env_change(env);
//         },
//         [this](env_change_atom, const environment& env) {
//            spdlog::info("session: env_change_atom");
//            on_env_change(env);
//            spdlog::info("session: env_change_atom done");
//         },
//         [this](meta_change_atom) {
//            spdlog::info("session: meta_change_atom 1");
//            if(Graph.refresh(context) != GRAPH_DSL_NS::Status::Ok) {
//               spdlog::error("session: refresh fail");
//               exit(nano_caf::exit_reason::unhandled_exception);
//            }
//            spdlog::info("session: env_change_atom 1 done");
//         },
//         [this](switch_done_atom) {
//            spdlog::info("session: switch_done_atom");
//            if(Graph.on_switch_done(context) != GRAPH_DSL_NS::Status::Ok) {
//               spdlog::error("session: on_switch_done fail");
//               exit(nano_caf::exit_reason::unhandled_exception);
//            }
//         },
//         [this](stop_atom) {
//            cleanup();
//            exit(nano_caf::exit_reason::normal);
//         },
//         [this](nano_caf::exit_msg_atom, nano_caf::exit_reason) {
//            cleanup();
//         }
//      };
//   }
//
//   auto on_env_change(const environment& env) -> void {
//      if(auto status = Graph.on_env_change(context, env); status != GRAPH_DSL_NS::Status::Ok) {
//         spdlog::error("session: refresh failed");
//         exit(nano_caf::exit_reason::unhandled_exception);
//      }
//   }
//
//   auto cleanup() -> void {
//      Graph.stop();
//      hw_1.exit();
//      hw_2.exit();
//   }
//
//   GRAPH_DSL_NS::GraphContext context;
//   md_graph Graph;
//   nano_caf::ActorHandle hw_1;
//   nano_caf::ActorHandle hw_2;
//};
//
//struct user_actor : nano_caf::actor {
//   explicit user_actor(nano_caf::ActorHandle session)
//      : session_{std::move(session)} {
//   }
//
//   ~user_actor() {
//   }
//
//   auto on_init()  -> void override {
//      user_op();
//   }
//
//   auto user_op() -> void {
//      std::random_device r;
//      std::default_random_engine regen{r()};
//      std::uniform_int_distribution<size_t> uniform(5, 10);
//
//      after(std::chrono::seconds(uniform(regen)), [this]{
//         environment env;
//         std::random_device r;
//         std::default_random_engine regen{r()};
//         std::uniform_int_distribution<size_t> uniform_1(0, 12);
//         std::uniform_int_distribution<size_t> uniform_2(1, 4);
//         env.condition_1 = 1.1 * uniform_2(regen);
//         env.condition_2 = uniform_1(regen);
//         env.condition_3 = uniform_1(regen);
//         session_.Send<env_change>(env);
//         user_op();
//      });
//   }
//
//private:
//   nano_caf::ActorHandle session_;
//};
//
//int main() {
//
//   nano_caf::ActorSystem::Instance().StartUp(1);
//
//   environment env;
//
//   auto session = Spawn<session_actor>();
//   session.Send<start>(env);
//
//   auto user = Spawn<user_actor>(session);
//
//   for(auto i=0; i<60; i++) {
//      session.Send<switch_done>();
//      std::this_thread::sleep_for(1s);
//      if(i % 5 == 0) {
//         node_condition = !node_condition;
//         session.Send<meta_change>();
//      }
//   }
//
//   std::this_thread::sleep_for(1s);
//
//   spdlog::info("ready for exit");
//   user.exit_and_wait();
//
//   session.Send<stop>();
//
//   spdlog::info("wait for session exit");
//   session.wait_for_exit();
//   session.release();
//
//   spdlog::info("system shutdown");
//   system.shutdown();
//
//   return 0;
//}
//
