//
// Created by Darwin Yuan on 2020/8/20.
//

#ifndef GRAPH_INTERVAL_H
#define GRAPH_INTERVAL_H

#include <graph/graph_ns.h>
#include <type_traits>
#include <maco/basic.h>
#include <maco/map.h>
#include <graph/util/number_string.h>
#include <cmath>

GRAPH_DSL_NS_BEGIN

template<typename ... Ts>
constexpr bool Is_Number_String = (std::is_base_of_v<num_s::number_string_signature, Ts> && ...);

template<typename T1, typename T2, typename = std::enable_if_t<Is_Number_String<T1, T2>>>
struct interval {
   static_assert(!T1::upper_boundary_tag , "invalid interval");
   static_assert(!T2::lower_boundary_tag , "invalid interval");

   constexpr static auto lower_boundary_open = !T1::lower_boundary_closed;
   constexpr static auto upper_boundary_open = !T2::upper_boundary_closed;

   constexpr static auto lower_boundary = T1::value;
   constexpr static auto upper_boundary = T2::value;
   static_assert(lower_boundary <= upper_boundary, "invalid interval");
   static_assert(std::is_convertible_v<decltype(lower_boundary), decltype(upper_boundary)> ||
                 std::is_convertible_v<decltype(upper_boundary), decltype(lower_boundary)>);
};

template<typename, typename ... Ts>
using interval_t = interval<Ts...>;

template<typename INTERVAL>
struct interval_trait {
private:
   // The following definitions are from <The art of computer programming> by Knuth:
   template<typename U, typename V>
   inline static constexpr auto equal(const U& lhs, const V& rhs) noexcept -> bool {
      return std::fabs(lhs - rhs) <= ( (std::fabs(lhs) > std::fabs(rhs) ? std::fabs(rhs) : std::fabs(lhs)) * std::numeric_limits<double>::epsilon());
   }

   template<typename U, typename V>
   inline static constexpr auto greater_than(const U& lhs, const V& rhs) noexcept -> bool {
      return (lhs - rhs) > ((std::fabs(lhs) < std::fabs(rhs) ? std::fabs(rhs) : std::fabs(lhs)) * std::numeric_limits<double>::epsilon());
   }

   template<typename U, typename V>
   inline static constexpr auto less_than(const U& lhs, const V& rhs) noexcept -> bool
   {
      return (rhs - lhs) > ( (std::fabs(lhs) < std::fabs(rhs) ? std::fabs(rhs) : std::fabs(lhs)) * std::numeric_limits<double>::epsilon());
   }

   template<typename U, typename V>
   inline static constexpr auto less_than_or_equal(const U& lhs, const V& rhs) noexcept -> bool {
      return less_than(lhs, rhs) || equal(lhs, rhs);
   }

   template<typename U, typename V>
   inline static constexpr auto greater_than_or_equal(const U& lhs, const V& rhs) noexcept -> bool {
      return greater_than(lhs, rhs) || equal(lhs, rhs);
   }

   template<typename T>
   inline static constexpr auto lower_match(const T& value) noexcept -> bool {
      if constexpr (std::is_floating_point_v<T> || std::is_floating_point_v<decltype(INTERVAL::lower_boundary)>) {
         if constexpr (INTERVAL::lower_boundary_open) {
            return greater_than(value, INTERVAL::lower_boundary);
         } else {
            return greater_than_or_equal(value, INTERVAL::lower_boundary);
         }
      } else {
         if constexpr (INTERVAL::lower_boundary_open) {
            return (value > INTERVAL::lower_boundary);
         } else {
            return (value >= INTERVAL::lower_boundary);
         }
      }
   }

   template<typename T>
   inline static constexpr auto upper_match(const T& value) noexcept -> bool {
      if constexpr (std::is_floating_point_v<T> || std::is_floating_point_v<decltype(INTERVAL::upper_boundary)>) {
         if constexpr (INTERVAL::upper_boundary_open) {
            return less_than(value, INTERVAL::upper_boundary);
         } else {
            return less_than_or_equal(value, INTERVAL::upper_boundary);
         }
      } else {
         if constexpr (INTERVAL::upper_boundary_open) {
            return (value < INTERVAL::upper_boundary);
         } else {
            return (value <= INTERVAL::upper_boundary);
         }
      }
   }

public:
   template<typename T>
   inline static constexpr auto contains(const T& value) noexcept -> bool {
      return lower_match(value) && upper_match(value);
   }
};

#define __CUB_interval_parameter(x) , __CUB_number_string(x)
#define __CUB_interval_only(...) \
GRAPH_DSL_NS::interval_t<void __MACO_map(__CUB_interval_parameter, __VA_ARGS__)>
#define __CUB_interval(...) GRAPH_DSL_NS::interval_trait<__CUB_interval_only(__VA_ARGS__)>

GRAPH_DSL_NS_END

#endif //GRAPH_INTERVAL_H
