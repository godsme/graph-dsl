//
// Created by Darwin Yuan on 2020/8/19.
//

#include <catch.hpp>
#include <graph/util/number_string.h>
#include <iostream>

namespace {

   TEST_CASE("number_string") {
      static_assert(__CUB_number_string(0)::value == 0);
      static_assert(__CUB_number_string(123)::value == 123);
      static_assert(__CUB_number_string(.0)::value == 0);
      static_assert(__CUB_number_string(0.)::value == 0);
      static_assert(__CUB_number_string(0.123)::value == 0.123);
      static_assert(__CUB_number_string(-0.123)::value == -0.123);
      static_assert(__CUB_number_string(-.123)::value == -0.123);
      static_assert(__CUB_number_string(-14.123)::value == -14.123);
      static_assert(__CUB_number_string(+0.123)::value == 0.123);
      static_assert(__CUB_number_string(+14.123)::value == 14.123);
      static_assert(__CUB_number_string(.234)::value == 0.234);
      static_assert(__CUB_number_string(2345.2345678)::value - 2345.2345678 < std::numeric_limits<double>::epsilon());
      static_assert(__CUB_number_string(15.84)::value == 15.84);
      static_assert(__CUB_number_string(26.)::value == 26);
      static_assert(std::is_base_of_v<GRAPH_DSL_NS::num_s::number_string_signature, __CUB_number_string(123)>);
      static_assert(std::is_base_of_v<GRAPH_DSL_NS::num_s::number_string_signature, __CUB_number_string(1.23)>);
   }
}