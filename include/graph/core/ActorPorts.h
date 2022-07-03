//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_ACTOR_PORTS_H
#define GRAPH_ACTOR_PORTS_H

#include <graph/graph_ns.h>
#include <nano-caf/actor/ActorHandle.h>
#include <tuple>
#include <vector>
#include <map>

GRAPH_DSL_NS_BEGIN

using PortId = uint8_t;

enum class ImageType : uint8_t {
   RGB,
   YUV
};

struct PortFormat {
   uint32_t   width;
   uint32_t   height;
   ImageType  imageType;
};

using ActorHandleSet = std::vector<nano_caf::ActorHandle>;

struct ActorPort {
    PortFormat m_format;
    ActorHandleSet m_actorHandles;

   template<typename MSG, typename ... Args>
   auto Send(Args&& ... args) {
      for(auto&& handle : m_actorHandles) {
         handle.Send<MSG>(std::forward<Args>(args)...);
      }
   }
};

using ActorPorts = std::map<PortId, ActorPort>;

GRAPH_DSL_NS_END

#endif //GRAPH_ACTOR_PORTS_H
