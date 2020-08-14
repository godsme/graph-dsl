//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_ASSERTION_H
#define GRAPH_ASSERTION_H

#include <graph/status.h>

#define GRAPH_EXPECT_SUCC_CALL(call) do { \
   auto status = call;              \
   if(status != status_t::ok) {     \
      return status;                \
   }                                \
} while(0)

#define GRPAH_EXPECT_TRUE(cond) do {      \
   if(!(cond)) {                    \
      return status_t::failed;      \
   }                                \
} while(0)

#endif //GRAPH_ASSERTION_H
