//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_ASSERTION_H
#define GRAPH_ASSERTION_H

#include <graph/status.h>

#define GRAPH_EXPECT_SUCC(call) do { \
   auto status = call;               \
   if(status != Status::Ok) {      \
      return status;                 \
   }                                 \
} while(0)

#define GRAPH_EXPECT_TRUE(cond) do { \
   if(!(cond)) {                     \
      return Status::Failed;       \
   }                                 \
} while(0)

#endif //GRAPH_ASSERTION_H
