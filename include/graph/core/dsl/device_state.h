//
// Created by Darwin Yuan on 2020/8/21.
//

#ifndef GRAPH_DEVICE_STATE_H
#define GRAPH_DEVICE_STATE_H

#include <graph/graph_ns.h>
#include <graph/util/hana_tuple_trait.h>
#include <graph/core/root_state.h>
#include <type_traits>
#include <holo/holo.h>

GRAPH_DSL_NS_BEGIN

///////////////////////////////////////////////////////////////////////////////
template<typename DEVICE>
struct preview_tag {
   constexpr static auto Device_Id = DEVICE::Device_Id;
   constexpr static auto Is_Preview = true;
};

namespace detail {
   template<typename DEVICE, typename = void>
   struct device_trait {
      constexpr static auto Device_Id = DEVICE::Device_Id;
      constexpr static auto Is_Preview = false;
   };

   template<typename DEVICE>
   struct device_trait<DEVICE, std::enable_if_t<DEVICE::Is_Preview>> : DEVICE {};
}

///////////////////////////////////////////////////////////////////////////////
template<typename ... DEVICEs>
struct device_state {
   constexpr static size_t Num_Of_Devices = sizeof...(DEVICEs);
   constexpr static auto Sorted_Devices =
      __HOLO_tuple_t<detail::device_trait<DEVICEs>...>
      | holo::sort([](auto l, auto r) {
         return holo::char_c<decltype(l)::type::Device_Id>
            <
            holo::char_c<decltype(r)::type::Device_Id>;
      });

   template<typename ... Ts>
   struct devices_type {
      constexpr static device_info Devices[] = {
         {Ts::Device_Id, Ts::Is_Preview}...
      };
      constexpr static root_state Root_State { .device_info_ = Devices, .size_ = Num_Of_Devices };
   };

   using devices = holo::tuple_trait_t<decltype(Sorted_Devices), devices_type>;
   constexpr static auto Devices = devices::Devices;

   constexpr static auto Root_State = devices::Root_State;

   template<typename DEVICE>
   inline static constexpr auto content_equal() -> bool {
      for(size_t i=0; i<Num_Of_Devices; i++) {
         if (Devices[i] != DEVICE::Devices[i]) {
            return false;
         }
      }
      return true;
   }

   template<typename DEVICE>
   inline static constexpr auto equals() noexcept {
      if constexpr (Num_Of_Devices != DEVICE::Num_Of_Devices) {
         return holo::bool_c<false>;
      } else {
         if constexpr (content_equal<DEVICE>()) {
            return holo::bool_c<true>;
         } else {
            return holo::bool_c<false>;
         }
      }
   }

   template<typename DEVICE>
   inline static constexpr auto content_less_than() -> bool {
      for(size_t i=0; i<Num_Of_Devices; i++) {
         if (Devices[i] >= DEVICE::Devices[i]) {
            return false;
         }
      }
      return true;
   }

   template<typename DEVICE>
   inline static constexpr auto less_than() noexcept {
      if constexpr (Num_Of_Devices < DEVICE::Num_Of_Devices) {
         return holo::bool_c<true>;
      } else if constexpr(Num_Of_Devices > DEVICE::Num_Of_Devices) {
         return holo::bool_c<false>;
      } else {
         if constexpr (content_less_than<DEVICE>()) {
            return holo::bool_c<true>;
         } else {
            return holo::bool_c<false>;
         }
      }
   }
};

GRAPH_DSL_NS_END

#define __g_PREVIEW(device)     GRAPH_DSL_NS::preview_tag<device>
#define __g_STATE(...)          std::decay_t<decltype(std::declval<GRAPH_DSL_NS::device_state<__VA_ARGS__>>())>

#endif //GRAPH_DEVICE_STATE_H
