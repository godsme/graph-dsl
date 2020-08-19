//
// Created by Darwin Yuan on 2020/8/20.
//

#include <catch.hpp>
#include <graph/util/number_string.h>
#include <graph/util/interval.h>

namespace {
   //using interval1 = __CUB_interval(2.0, 18);

   TEST_CASE("interval open") {
      REQUIRE_FALSE(__CUB_interval(2.0, 18)::contains(2.0));
      REQUIRE_FALSE(__CUB_interval(2.0, 18)::contains(18));
      REQUIRE_FALSE(__CUB_interval(2.0, 18)::contains(1.999999999999));
      REQUIRE_FALSE(__CUB_interval(2.0, 18)::contains(18.0000000000001));
   }

   TEST_CASE("interval closed") {
      REQUIRE(__CUB_interval( [2.0, 18] )::contains(2.0));
      REQUIRE(__CUB_interval( [2.0, 18] )::contains(18));
      REQUIRE_FALSE(__CUB_interval( [2.0, 18] )::contains(1.999999999999));
      REQUIRE_FALSE(__CUB_interval( [2.0, 18] )::contains(18.0000000000001));
   }

   TEST_CASE("lower open interval") {
      REQUIRE_FALSE(__CUB_interval( {2.0, 18] )::contains(2.0));
      REQUIRE(__CUB_interval( {2.0, 18] )::contains(18));
      REQUIRE_FALSE(__CUB_interval( {2.0, 18] )::contains(1.999999999999));
      REQUIRE_FALSE(__CUB_interval( {2.0, 18] )::contains(18.0000000000001));
   }

   TEST_CASE("upper open interval") {
      REQUIRE(__CUB_interval( [2.0, 18.0} )::contains(2.0));
      REQUIRE_FALSE(__CUB_interval( [2.0, 18.0} )::contains(18));
      REQUIRE_FALSE(__CUB_interval( [2.0, 18.0} )::contains(1.999999999999));
      REQUIRE_FALSE(__CUB_interval( [2.0, 18.0} )::contains(18.0000000000001));
   }

   TEST_CASE("both open interval") {
      REQUIRE_FALSE(__CUB_interval( {2.1, 18.6} )::contains(2.1));
      REQUIRE_FALSE(__CUB_interval( {2.1, 18.6} )::contains(18.6));
      REQUIRE_FALSE(__CUB_interval( {2.1, 18.6} )::contains(2.09999999999999));
      REQUIRE_FALSE(__CUB_interval( {2.1, 18.6} )::contains(18.6000000000001));
   }
}